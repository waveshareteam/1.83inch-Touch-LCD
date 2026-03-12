#include <Arduino_GFX_Library.h>
#include "cst816d.h"

/*
 ** LCD_CS:   9
 ** LCD_DC:   8
 ** LCD_RST:  7
 ** LCD_BL:   6
 ** SCLK:     13
 ** MOSI:     11
*/

#define GFX_BL DF_GFX_BL  // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#define TOUCH_RST_PIN 2
#define TOUCH_INT_PIN 3

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ST7789(bus, 7, 0 /* rotation */, true /* IPS */, 240, 284);

void LCD_display_test(void) {
  gfx->fillScreen(RED);
  delay(1000);
  gfx->fillScreen(GREEN);
  delay(1000);
  gfx->fillScreen(BLUE);
  delay(1000);
  gfx->fillScreen(WHITE);
}

void setup(void) {

    Serial.begin(115200);
  Serial.println("Arduino_GFX Hello World example");

  // 初始化屏幕前打印提示
  Serial.println("Initializing display...");
  if (!gfx->begin()) {
    Serial.println("gfx->begin() FAILED! Check display connections.");
    // 持续打印错误，方便观察
    while(1) {
      delay(1000);
      Serial.println("Display init failed!");
    }
  } else {
    Serial.println("Display initialized successfully!");
  }



  // 再开启背光
#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  gfx->setCursor(10, 240);
  gfx->setTextSize(3);
  gfx->setTextColor(RED);
  gfx->println("Hello Waveshare!");
  delay(1000);
  LCD_display_test();
  cst816d_init(TOUCH_RST_PIN, -1);
}

touch_data_t touch_data;
void loop() {
  if (get_touch_data(&touch_data)) {
    Serial.print("x:");
    Serial.print(touch_data.coords[0].x);
    Serial.print("y:");
    Serial.println(touch_data.coords[0].y);
    gfx->drawCircle(touch_data.coords[0].x, touch_data.coords[0].y, 2, RED);
  }

  delay(10);
}