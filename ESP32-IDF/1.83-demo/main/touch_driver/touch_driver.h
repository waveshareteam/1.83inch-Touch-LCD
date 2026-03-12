#ifndef TOUCH_DRIVER_H  
#define TOUCH_DRIVER_H

#include "esp_lcd_touch.h"  
#include "lvgl_driver.h"
#include "esp_lcd_touch_cst816s.h"


/* Touch settings */
#define EXAMPLE_TOUCH_I2C_NUM          (0)
#define EXAMPLE_TOUCH_I2C_CLK_HZ       (400000)

/* LCD touch pins */
#define EXAMPLE_TOUCH_I2C_SCL          (GPIO_NUM_14)
#define EXAMPLE_TOUCH_I2C_SDA          (GPIO_NUM_15)
#define EXAMPLE_TOUCH_GPIO_INT         (GPIO_NUM_13)
#define EXAMPLE_TOUCH_GPIO_RST         (GPIO_NUM_39)

extern esp_lcd_touch_handle_t touch_handle;

esp_err_t touch_init(void);
void touch_test(void);

#endif  // TOUCH_DRIVER_H

