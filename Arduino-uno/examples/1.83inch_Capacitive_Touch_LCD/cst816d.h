#include <stdint.h>
#pragma once

#include <Arduino.h>
#include <Wire.h>

#define TP_I2C_FREQ 400000
#define CST816D_ADDR 0x15
#define CST816D_ID_REG 0xA7

#define CST816D_TOUCH_NUM_REG 0X02
#define CST816D_TOUCH_XH_REG 0x03
#define CST816D_TOUCH_XL_REG 0x04
#define CST816D_TOUCH_YH_REG 0x05
#define CST816D_TOUCH_YL_REG 0x06


#define CST816D_TOUCH_MAX_NUM   2


typedef struct{
    uint16_t x;
    uint16_t y;
}touch_coords_t;

typedef struct{
  touch_coords_t coords[CST816D_TOUCH_MAX_NUM];
  uint16_t touch_num;
}touch_data_t;


void cst816d_init(int tp_rst, int tp_int);
bool get_touch_data(touch_data_t *touch_data);

void cst816d_touch_int_cb(void);

