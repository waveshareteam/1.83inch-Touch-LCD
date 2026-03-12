#include "touch_driver.h"

bool touch_test_done = false;
esp_lcd_touch_handle_t touch_handle = NULL;
i2c_master_bus_handle_t i2c_bus_handle = NULL;



esp_err_t touch_init(void)
{

    i2c_master_bus_config_t i2c_mst_config = {};
    i2c_mst_config.clk_source = I2C_CLK_SRC_DEFAULT;
    i2c_mst_config.i2c_port = (i2c_port_num_t)EXAMPLE_TOUCH_I2C_NUM;
    i2c_mst_config.scl_io_num = EXAMPLE_TOUCH_I2C_SCL;
    i2c_mst_config.sda_io_num = EXAMPLE_TOUCH_I2C_SDA;
    i2c_mst_config.glitch_ignore_cnt = 7;
    i2c_mst_config.flags.enable_internal_pullup = 1;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle));

    esp_lcd_panel_io_handle_t touch_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t touch_io_config = {};
    touch_io_config.dev_addr = ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS,
    touch_io_config.control_phase_bytes = 1;
    touch_io_config.dc_bit_offset = 0;
    touch_io_config.lcd_cmd_bits = 8;
    touch_io_config.flags.disable_control_phase = 1;
    touch_io_config.scl_speed_hz = 400 * 1000;

    /* Touch IO handle */
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &touch_io_config, &touch_io_handle));
    esp_lcd_touch_config_t tp_cfg = {};
    tp_cfg.x_max = LCD_H_RES;          
    tp_cfg.y_max = LCD_V_RES;          
    tp_cfg.rst_gpio_num = EXAMPLE_TOUCH_GPIO_RST;
    tp_cfg.int_gpio_num = EXAMPLE_TOUCH_GPIO_INT;
    tp_cfg.flags.swap_xy = 0;
    tp_cfg.flags.mirror_x = 0;
    tp_cfg.flags.mirror_y = 0;
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