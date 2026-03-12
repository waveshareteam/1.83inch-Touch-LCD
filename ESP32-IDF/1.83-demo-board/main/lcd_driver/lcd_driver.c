#include "lcd_driver.h"
#include "esp_log.h"
#include "string.h"

static const char *TAG = "LCD_DRIVER";       
static esp_lcd_panel_io_handle_t lcd_io = NULL;  // LCD panel IO handle (SPI communication layer)
esp_lcd_panel_handle_t lcd_panel = NULL;  // LCD panel handle (ST7789 driver layer)


void lcd_reset(void)
{
    Set_EXIO(IO_EXPANDER_PIN_NUM_0,0);
    vTaskDelay(pdMS_TO_TICKS(10));
    Set_EXIO(IO_EXPANDER_PIN_NUM_0,1);
    vTaskDelay(pdMS_TO_TICKS(50));
}

esp_lcd_panel_io_handle_t lcd_get_panel_io_handle(void)
{
    return lcd_io;  // Return the lcd_io created during initialization
}

esp_lcd_panel_handle_t lcd_get_panel_handle(void)
{
    return lcd_panel;  // Return the lcd_panel created during initialization
}




void lcd_fill_screen(esp_lcd_panel_handle_t panel_handle, uint16_t color)
{
    uint32_t pixel_count = LCD_H_RES * LCD_V_RES;
    uint8_t *color_buf = heap_caps_malloc(pixel_count * 2, MALLOC_CAP_DMA);
    if (color_buf == NULL) {
        ESP_LOGE("LCD", "malloc failed");
        return;
    }

    uint8_t color_high = (color >> 8) & 0xFF; 
    uint8_t color_low = color & 0xFF;         

    for (uint32_t i = 0; i < pixel_count; i++) {
        color_buf[i * 2]     = color_high;
        color_buf[i * 2 + 1] = color_low;
    }
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0,LCD_H_RES, LCD_V_RES, (uint16_t *)color_buf);
    free(color_buf);
}



static esp_err_t lcd_backlight_init(void)
{
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_GPIO_BL
    };
    return gpio_config(&bk_gpio_config);
}


static esp_err_t lcd_spi_bus_init(void)
{
    const spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_GPIO_SCLK,
        .mosi_io_num = LCD_GPIO_MOSI,
        .miso_io_num = GPIO_NUM_NC,  // ST7789 doesn't need MISO (simplex communication)
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = LCD_H_RES * LCD_V_RES * sizeof(uint16_t) 
    };
    return spi_bus_initialize(LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO);
}

esp_err_t lcd_init(void)
{
    esp_err_t ret = ESP_OK;
    lcd_reset();
    // 1. Initialize backlight pin
    ESP_GOTO_ON_ERROR(lcd_backlight_init(), err, TAG, "Backlight init failed");

    // 2. Initialize SPI bus
    ESP_LOGD(TAG, "Initialize SPI bus for ST7789");
    ESP_GOTO_ON_ERROR(lcd_spi_bus_init(), err, TAG, "SPI bus init failed");

    // 3. Install LCD panel IO (SPI protocol adaptation)
    ESP_LOGD(TAG, "Install LCD panel IO (SPI)");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_GPIO_DC,
        .cs_gpio_num = LCD_GPIO_CS,
        .pclk_hz = LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_NUM, &io_config, &lcd_io), 
                      err, TAG, "Panel IO init failed");

    // 4. Install ST7789 LCD driver
    ESP_LOGD(TAG, "Install ST7789 panel driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_GPIO_RST,
        .color_space = LCD_COLOR_SPACE,
        .bits_per_pixel = LCD_BITS_PER_PIXEL
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_st7789(lcd_io, &panel_config, &lcd_panel), 
                      err, TAG, "ST7789 driver init failed");

    // 5. Reset and initialize ST7789 panel
    ESP_LOGD(TAG, "Reset and init ST7789 panel");
    ESP_GOTO_ON_ERROR(esp_lcd_panel_reset(lcd_panel), err, TAG, "Panel reset failed");
    ESP_GOTO_ON_ERROR(esp_lcd_panel_init(lcd_panel), err, TAG, "Panel init failed");
    
    // ST7789 has default color inversion; enable color correction
    ESP_GOTO_ON_ERROR(esp_lcd_panel_invert_color(lcd_panel, true), err, TAG, "Invert color failed");
    // Turn on LCD display
    ESP_GOTO_ON_ERROR(esp_lcd_panel_disp_on_off(lcd_panel, true), err, TAG, "Turn on display failed");
    lcd_fill_screen(lcd_panel, 0x0000);
    vTaskDelay(pdMS_TO_TICKS(500));

    // 6. Turn on LCD backlight
    vTaskDelay(pdMS_TO_TICKS(500));
    ESP_LOGD(TAG, "Turn on LCD backlight");
    ESP_GOTO_ON_ERROR(gpio_set_level(LCD_GPIO_BL, LCD_BL_ON_LEVEL), err, TAG, "Turn on backlight failed");

    ESP_LOGI(TAG, "ST7789 LCD init success");
    return ret;

err:
    // Initialization failed: Release allocated resources to avoid memory leak
    if (lcd_panel != NULL) {
        esp_lcd_panel_del(lcd_panel);
        lcd_panel = NULL;
    }
    if (lcd_io != NULL) {
        esp_lcd_panel_io_del(lcd_io);
        lcd_io = NULL;
    }
    spi_bus_free(LCD_SPI_NUM);
    ESP_LOGE(TAG, "ST7789 LCD init failed (err: %s)", esp_err_to_name(ret));
    return ret;
}