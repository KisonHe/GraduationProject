#include "lv_control_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include <freertos/timers.h>
#include "../globals.h"
#include <rm_can_motors.h>
#include "gui.h"
extern canMotors::motor M3508;

static lv_obj_t * ignoreCMDSW = nullptr;
static lv_obj_t * safeSW = nullptr;
bool ignoreCMD = false;
bool setSafe = false;
static lv_style_t styleRedBG;
static lv_style_t styleGreenBG;

static void lv_control_style_init(){
    lv_style_init(&styleRedBG);
    lv_style_set_bg_color(&styleRedBG,lv_palette_main(LV_PALETTE_RED));

    lv_style_init(&styleGreenBG);
    lv_style_set_bg_color(&styleGreenBG,lv_palette_main(LV_PALETTE_GREEN));
}

static void lv_control_override_sw(lv_obj_t* view){
    ignoreCMDSW = lv_switch_create(view);
    lv_obj_add_event_cb(ignoreCMDSW, [](lv_event_t* e){
        ignoreCMD = lv_obj_has_state(ignoreCMDSW, LV_STATE_CHECKED) ? true : false;
    },LV_EVENT_ALL,nullptr);

    lv_obj_set_align(ignoreCMDSW,LV_ALIGN_TOP_LEFT);
    lv_obj_t * ignoreCMDSWLabel = lv_label_create(view);
    lv_obj_align_to(ignoreCMDSWLabel, ignoreCMDSW, LV_ALIGN_OUT_RIGHT_MID, 4, 0);
    lv_obj_set_style_text_font(ignoreCMDSWLabel,p_custom_font,0);
    lv_obj_add_style(ignoreCMDSW,&styleRedBG,LV_STATE_DEFAULT);
    lv_obj_add_style(ignoreCMDSW,&styleGreenBG,LV_STATE_CHECKED|LV_PART_INDICATOR);
    lv_label_set_text(ignoreCMDSWLabel,"超越命令");
}

static void lv_control_safe_sw(lv_obj_t* view){
    safeSW = lv_switch_create(view);
    lv_obj_add_event_cb(safeSW, [](lv_event_t* e){
        setSafe = lv_obj_has_state(safeSW, LV_STATE_CHECKED) ? true : false;
    },LV_EVENT_ALL,nullptr);
    lv_obj_add_style(safeSW,&styleRedBG,LV_STATE_DEFAULT);
    lv_obj_add_style(safeSW,&styleGreenBG,LV_STATE_CHECKED|LV_PART_INDICATOR);

    lv_obj_set_align(safeSW,LV_ALIGN_TOP_RIGHT);
    lv_obj_t * safeSWLabel = lv_label_create(view);
    lv_obj_set_style_text_font(safeSWLabel,p_custom_font,0);
    lv_label_set_text(safeSWLabel,"安全模式");
    lv_obj_align_to(safeSWLabel, safeSW, LV_ALIGN_OUT_LEFT_MID, -lv_obj_get_width(safeSWLabel), 0);
}

static void lv_control_rpm_sld(lv_obj_t* view){

}

static void lv_control_pos_sld(lv_obj_t* view){

}

void lv_control_tab_init(lv_obj_t* view){
    lv_control_style_init();
    lv_control_override_sw(view);
    lv_control_rpm_sld(view);
    lv_control_safe_sw(view);
}
