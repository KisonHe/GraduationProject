#pragma once
#include <lvgl.h>


/*2: Stored in SPIFFS as "/spiffs/customfont.bin"
 *1: Format LVGL https://github.com/lvgl/lv_font_conv#cli-params; Simply Name the font to custom_font
 *0: No CJK Font, will set cjk_font to LV_FONT_DEFAULT*/
#define CUSTOM_FONT_TYPE 2

extern const lv_font_t* p_custom_font;