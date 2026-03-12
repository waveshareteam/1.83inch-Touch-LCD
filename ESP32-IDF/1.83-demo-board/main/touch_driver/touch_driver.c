#include "touch_driver.h"

bool touch_test_done = false;
esp_lcd_touch_handle_t touch_handle = NULL;
extern i2c_master_bus_handle_t i2c_bus_handle;

esp_err_t i2c_bus_init(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = (i2c_port_num_t)EXAMPLE_TOUCH_I2C_NUM,
        .scl_io_num = EXAMPLE_TOUCH_I2C_SCL,
        .sda_io_num = EXAMPLE_TOUCH_I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = 1,
    };
    return i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle);
}

void touch_reset(void)
{
    Set_EXIO(IO_EXPANDER_PIN_NUM_1,0);
    vTaskDelay(pdMS_TO_TICKS(50));
    Set_EXIO(IO_EXPANDER_PIN_NUM_1,1);
    vTaskDelay(pdMS_TO_TICKS(50));
}

esp_err_t touch_init(void)
{
    touch_reset();   

    esp_lcd_panel_io_handle_t touch_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t touch_io_config = {
        .dev_addr = ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS,
        .control_phase_bytes = 1,
        .dc_bit_offset = 0,
        .lcd_cmd_bits = 8,
        .flags.disable_control_phase = 1,
        .scl_speed_hz = 400 * 1000,
    };

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &touch_io_config, &touch_io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = EXAMPLE_TOUCH_GPIO_RST,
        .int_gpio_num = EXAMPLE_TOUCH_GPIO_INT,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    return esp_lcd_touch_new_i2c_cst816s(touch_io_handle, &tp_cfg, &touch_handle);
}


void touch_test(void)
{
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;
    uint16_t color_arr[16] = {0};

    for (int i = 0; i < 16; i++)
    {
        color_arr[i] = 0xf800;
    }

    vTaskDelay(pdMS_TO_TICKS(500));
        while (!touch_test_done)
        {
            /* Read data from touch controller into memory */
            esp_lcd_touch_read_data(touch_handle);

            /* Read data from touch controller */
            bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_handle, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);
            if (touchpad_pressed && touchpad_cnt > 0)
            {
                if (touchpad_x[0] < 2)
                    touchpad_x[0] = 2;
                else if (touchpad_x[0] > LCD_H_RES - 2 - 1)
                    touchpad_x[0] = LCD_H_RES - 2 - 1;

                if (touchpad_y[0] < 2)
                    touchpad_y[0] = 2;
                else if (touchpad_y[0] > LCD_V_RES - 2 - 1)
                    touchpad_y[0] = LCD_H_RES - 2 - 1;

                esp_lcd_panel_draw_bitmap(lcd_panel, touchpad_x[0] - 2, touchpad_y[0] - 2, touchpad_x[0] + 2, touchpad_y[0] + 2, color_arr);
            }
            vTaskDelay(pdMS_TO_TICKS(10));
        }
}