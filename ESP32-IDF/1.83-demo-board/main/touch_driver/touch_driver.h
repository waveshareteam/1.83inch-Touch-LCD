#ifndef TOUCH_DRIVER_H  
#define TOUCH_DRIVER_H

#include "tca9555_driver.h"
#include "esp_lcd_touch.h"  
#include "lvgl_driver.h"
#include "esp_lcd_touch_cst816s.h"


/* Touch settings */
#define EXAMPLE_TOUCH_I2C_NUM          (0)
#define EXAMPLE_TOUCH_I2C_CLK_HZ       (400000)

/* LCD touch pins */
#define EXAMPLE_TOUCH_I2C_SCL          (GPIO_NUM_10)
#define EXAMPLE_TOUCH_I2C_SDA          (GPIO_NUM_11)
#define EXAMPLE_TOUCH_GPIO_INT         (GPIO_NUM_NC)
#define EXAMPLE_TOUCH_GPIO_RST         (GPIO_NUM_NC)

extern esp_lcd_touch_handle_t touch_handle;

void touch_reset(void);
void touch_test(void);
esp_err_t touch_init(void);
esp_err_t i2c_bus_init(void);

#endif  // TOUCH_DRIVER_H

