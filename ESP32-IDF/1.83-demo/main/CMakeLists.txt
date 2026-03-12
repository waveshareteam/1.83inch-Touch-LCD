# 初始化源文件和包含目录
set(SOURCES
    "./main.c"
    "./lcd_driver/lcd_driver.c"
    "./touch_driver/touch_driver.c"
    "./lvgl_driver/lvgl_driver.c"
    "./image1.c"
    "./image2.c"
    "./image3.c"
)

set(INCLUDE_DIRS
    "."
    "./lcd_driver"
    "./lvgl_driver"
    "./touch_driver"
)

# 注册组件
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
)