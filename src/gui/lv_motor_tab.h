#pragma once
#include <lvgl.h>
#include <FreeRTOS.h>
void lv_motor_tab_init(lv_obj_t* view);
void lv_motor_tab_meter_update();
void lv_motor_tab_label_update();
extern SemaphoreHandle_t update_motor_tab_meter;
extern SemaphoreHandle_t update_motor_tab_label;
