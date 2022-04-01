#include "lv_motor_tab.h"


static lv_obj_t * motorTab = nullptr;

void lv_motor_tab_init(lv_obj_t* view){
    lv_tabview_add_tab(view, "电机");
}