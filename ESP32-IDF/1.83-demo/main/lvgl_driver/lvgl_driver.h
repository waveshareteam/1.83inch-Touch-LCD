#ifndef LVGL_DRIVER_H  
#define LVGL_DRIVER_H

#include "esp_lvgl_port.h"
#include "lcd_driver.h"
#include "touch_driver.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

extern lv_display_t *lvgl_disp;
extern lv_indev_t *lvgl_touch_indev;

esp_err_t lvgl_init(void);

#endif  