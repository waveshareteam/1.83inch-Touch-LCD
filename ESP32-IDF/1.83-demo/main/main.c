/********************************************************************************
 * Author: Javen
 * Company: Waveshare
 * Modification Date: 2025/12/2
 * Description: LVGL 3-image gesture album (ESP32 + ST7789 LCD + CST816 touch, left/right swipe cycle)
 ********************************************************************************/
#include "string.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"  
#include "esp_lcd_panel_ops.h" 
#include "esp_lvgl_port.h"
#include "esp_lcd_touch_cst816s.h"
#include "demos/lv_demos.h"
#include "lcd_driver.h"
#include "lvgl_driver.h"
#include "touch_driver.h"


static const char *TAG = "EXAMPLE";

// Image object pointers (used as display image widgets)
static lv_obj_t *img1_obj;
static lv_obj_t *img2_obj;
static lv_obj_t *img3_obj;


// Index of currently displayed image
static int8_t current_img = 0;

// Image resources (LVGL image data structure)
LV_IMG_DECLARE(image1);
LV_IMG_DECLARE(image2);
LV_IMG_DECLARE(image3);


static void switch_to_image(int index) 
{
    lv_obj_add_flag(img1_obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(img2_obj, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(img3_obj, LV_OBJ_FLAG_HIDDEN);
    
    switch(index) {
        case 0:
            lv_obj_clear_flag(img1_obj, LV_OBJ_FLAG_HIDDEN);
            break;
        case 1:
            lv_obj_clear_flag(img2_obj, LV_OBJ_FLAG_HIDDEN);
            break;
        case 2:
            lv_obj_clear_flag(img3_obj, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

static void albumpage_gesture_cb(lv_event_t * e)
{

    lv_event_code_t event = lv_event_get_code(e);
    if(event == LV_EVENT_GESTURE) {
        lv_indev_wait_release(lv_indev_get_act());
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        switch(dir){
            case LV_DIR_LEFT:
                current_img--;
                if(current_img<0)
                {
                    current_img = 2;
                }
                switch_to_image(current_img);
                printf("to left\n");
                break;
            case LV_DIR_RIGHT:
                current_img++;
                if(current_img>2)
                {
                    current_img = 0;
                }
                switch_to_image(current_img);
                printf("to right\n");
                break;
            default:break;
        }
    }
}


void image_slider_init(void) 
{

    lv_obj_t *ui_albumpage = lv_obj_create(lv_scr_act());
    lv_obj_clear_flag(ui_albumpage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(ui_albumpage, LV_HOR_RES, LV_VER_RES);       
    lv_obj_set_style_bg_color(ui_albumpage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_albumpage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(ui_albumpage,0,LV_STATE_DEFAULT);
    lv_obj_center(ui_albumpage);

    // Create image object 1
    img1_obj = lv_image_create(ui_albumpage);
    lv_img_set_src(img1_obj, &image1);  
    lv_obj_center(img1_obj);
   
    lv_obj_set_align(img1_obj, LV_ALIGN_CENTER);
    lv_obj_add_flag(img1_obj, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_remove_flag(img1_obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(lv_scr_act(), albumpage_gesture_cb, LV_EVENT_GESTURE, NULL);


    // Create image object 2 (initially hidden)
    img2_obj = lv_image_create(ui_albumpage);
    lv_img_set_src(img2_obj, &image2);
    lv_obj_center(img2_obj);
    lv_obj_add_flag(img2_obj, LV_OBJ_FLAG_HIDDEN);
    
    // Create image object 3 (initially hidden)
    img3_obj = lv_image_create(ui_albumpage);
    lv_img_set_src(img3_obj, &image3);
    lv_obj_center(img3_obj);
    lv_obj_add_flag(img3_obj, LV_OBJ_FLAG_HIDDEN);
    
    // Display the first image initially
    current_img = 0;
}




void app_main(void)
{
    lcd_init();
    touch_init();
    lvgl_init();
    // lvgl_port_lock(0);
    // if (touch_handle != NULL)
    // {
    //     image_slider_init();
    // }
    // lvgl_port_unlock();
    // touch_test();

    while(1) {

        lcd_fill_screen(lcd_panel, 0xF800); // 填充红色
        vTaskDelay(pdMS_TO_TICKS(2000));

        lcd_fill_screen(lcd_panel, 0x07E0); // 填充绿色
        vTaskDelay(pdMS_TO_TICKS(2000));

        lcd_fill_screen(lcd_panel, 0x001F); // 填充蓝色
        vTaskDelay(pdMS_TO_TICKS(2000));

        lcd_fill_screen(lcd_panel, 0xFFE0); // 填充黄色
        vTaskDelay(pdMS_TO_TICKS(2000));

        lcd_fill_screen(lcd_panel, 0xFFFF); // 填充白色
        touch_test();
    }
}