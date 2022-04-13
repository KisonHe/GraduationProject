#pragma once
#include <lvgl.h>
#include <FreeRTOS.h>
extern SemaphoreHandle_t update_sensor_tab_label;
void lv_sensor_tab_init(lv_obj_t* view);
void lv_sensor_tab_label_update();
