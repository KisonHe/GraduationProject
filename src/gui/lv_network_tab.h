#pragma once
#include <lvgl.h>
#include <FreeRTOS.h>
extern SemaphoreHandle_t update_network_tab_label;
void lv_network_tab_init(lv_obj_t* view);
void lv_network_tab_label_update();
