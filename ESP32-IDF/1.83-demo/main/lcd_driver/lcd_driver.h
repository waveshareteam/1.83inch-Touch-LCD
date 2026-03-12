#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_check.h"


/* LCD Dimensions */
#define LCD_H_RES          (240)    // LCD horizontal resolution
#define LCD_V_RES          (284)    // LCD vertical resolution

/* LCD SPI Configuration */
#define LCD_SPI_NUM        (SPI3_HOST)       // SPI host to use
#define LCD_PIXEL_CLK_HZ   (40 * 1000 * 1000) // SPI clock frequency (40MHz)
#define LCD_CMD_BITS       (8)                // Command bit width
#define LCD_PARAM_BITS     (8)                // Parameter bit width
#define LCD_COLOR_SPACE    (ESP_LCD_COLOR_SPACE_RGB) // Color space
#define LCD_BITS_PER_PIXEL (16)               // Pixel bit width (RGB565 format)
#define LCD_BL_ON_LEVEL    (1)                // Backlight on level (1=high level, 0=low level)

/* LCD Pin Definitions */
#define LCD_GPIO_SCLK      (GPIO_NUM_6) // SPI clock pin
#define LCD_GPIO_MOSI      (GPIO_NUM_7) // SPI data output pin (ST7789 simplex, MISO not required)
#define LCD_GPIO_RST       (GPIO_NUM_38) // Reset pin
#define LCD_GPIO_DC        (GPIO_NUM_4) // Command/data distinction pin
#define LCD_GPIO_CS        (GPIO_NUM_5) // Chip select pin
#define LCD_GPIO_BL        (GPIO_NUM_40) // Backlight pin

extern esp_lcd_panel_handle_t lcd_panel;
esp_err_t lcd_init(void);
esp_lcd_panel_io_handle_t lcd_get_panel_io_handle(void);
esp_lcd_panel_handle_t lcd_get_panel_handle(void);
void lcd_fill_screen(esp_lcd_panel_handle_t panel_handle, uint16_t color);


#endif