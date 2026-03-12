#include <stdint.h>
#include "cst816d.h"


static bool cst816d_read_flag = true;
static bool cst816d_int_enable = false;

static bool cst816d_write(uint8_t driver_addr, uint8_t reg_addr, const uint8_t *data, uint32_t length) {
  Wire.beginTransmission(driver_addr);
  Wire.write(reg_addr);
  Wire.write(data, length);


  if (Wire.endTransmission() != 0) {
    Serial.println("The I2C transmission fails. - I2C Read\r\n");
    return false;
  }
  return true;
}

static bool cst816d_read(uint8_t driver_addr, uint8_t reg_addr, uint8_t *data, uint32_t length) {
  Wire.beginTransmission(driver_addr);
  Wire.write(reg_addr);
  if (Wire.endTransmission() != 0) {
    Serial.println("The I2C write fails. - I2C Read\r\n");
    return false;
  }

  Wire.requestFrom(driver_addr, length);
  if (Wire.available() != length) {
    Serial.println("The I2C read fails. - I2C Read\r\n");
    return false;
  }
  Wire.readBytes(data, length);
  return true;  // 读取成功
}




bool get_touch_data(touch_data_t *touch_data) {
  uint8_t buff[12];
  uint8_t touch_num = 0;

  if (cst816d_int_enable) {
    if (false == cst816d_read_flag) {
      // Serial.println("cst816d_read_flag is false");
      return false;
    } else {
      cst816d_read_flag = false;
    }
  }

  cst816d_read(CST816D_ADDR, CST816D_TOUCH_NUM_REG, &touch_num, 1);
  touch_data->touch_num = touch_num;
  if (touch_num == 0)
    return false;

  // cst816d_read(CST816D_ADDR, CST816D_TOUCH_XH_REG, buff, touch_num * 6);
  cst816d_read(CST816D_ADDR, CST816D_TOUCH_XH_REG, &buff[0], 1);
  cst816d_read(CST816D_ADDR, CST816D_TOUCH_XL_REG, &buff[1], 1);
  cst816d_read(CST816D_ADDR, CST816D_TOUCH_YH_REG, &buff[2], 1);
  cst816d_read(CST816D_ADDR, CST816D_TOUCH_YL_REG, &buff[3], 1);
  for (uint16_t i = 0; i < touch_num; i++) {
    touch_data->coords[i].x = (((uint16_t)buff[(i * 6) + 0] & 0x0f) << 8) + buff[(i * 6) + 1];
    touch_data->coords[i].y = (((uint16_t)buff[(i * 6) + 2] & 0x0f) << 8) + buff[(i * 6) + 3];
  }
  return true;
}

// 中断服务函数
void cst816d_touch_int_cb(void) {
  cst816d_read_flag = true;
}


void cst816d_init(int tp_rst, int tp_int) {
  uint8_t id = 0;
  // 初始化i2C
  Wire.begin();

  if (0 < tp_int) {
    // 复位
    digitalWrite(tp_rst, LOW);
    delay(200);
    digitalWrite(tp_rst, HIGH);
    delay(300);
  }
  if (0 < tp_int) {
    cst816d_int_enable = true;
    attachInterrupt(digitalPinToInterrupt(tp_int), cst816d_touch_int_cb, RISING); // 关联触控中断函数
  }
  cst816d_read(CST816D_ADDR, CST816D_ID_REG, &id, 1);
  if (0 != id) {
    Serial.print("读取成功:");
    Serial.println(id);
  }
  return true;
}