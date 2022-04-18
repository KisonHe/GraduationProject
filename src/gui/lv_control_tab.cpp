#include "lv_control_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include <freertos/timers.h>
#include "../globals.h"
#include <rm_can_motors.h>
#include "gui.h"
#include <Arduino.h>
extern canMotors::motor M3508;

static lv_obj_t * ignoreCMDSW = nullptr;
static lv_obj_t * safeSW = nullptr;
bool ignoreCMD = false;
bool setSafe = false;
static lv_style_t styleRedBG;
static lv_style_t styleGreenBG;
static lv_obj_t * rpm_sld = nullptr;
static lv_obj_t * pos_spb = nullptr;
static lv_obj_t * rpm_sld_value_label = nullptr;
static lv_obj_t* rpm_sld_name_label;
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
        if (setSafe){
            M3508.Speed_Set(0); //For Updates to work
            M3508.Safe_Set();
        }

    },LV_EVENT_VALUE_CHANGED,nullptr);
    lv_obj_add_style(safeSW,&styleRedBG,LV_STATE_DEFAULT);
    lv_obj_add_style(safeSW,&styleGreenBG,LV_STATE_CHECKED|LV_PART_INDICATOR);

    lv_obj_set_align(safeSW,LV_ALIGN_TOP_RIGHT);
    lv_obj_t * safeSWLabel = lv_label_create(view);
    lv_obj_set_style_text_font(safeSWLabel,p_custom_font,0);
    lv_label_set_text(safeSWLabel,"安全模式");
    lv_obj_align_to(safeSWLabel, safeSW, LV_ALIGN_OUT_LEFT_MID, -(lv_obj_get_width(safeSWLabel) + 4), 0);
}

static void slider_event_cb(lv_event_t * e){
    lv_obj_t * slider = lv_event_get_target(e);
    char buf[18];
    lv_snprintf(buf, sizeof(buf), "%d00 RPM", (int)lv_slider_get_value(slider));
    lv_label_set_text(rpm_sld_value_label, buf);
    if (!setSafe){
        M3508.Speed_Set((int)lv_slider_get_value(slider)*100);   //Not thread safe but whatever this is just rubbish graduation code
    }
}

static void lv_control_rpm_sld(lv_obj_t* view){
    rpm_sld_name_label = lv_label_create(view);
    lv_obj_set_style_text_font(rpm_sld_name_label,p_custom_font,0);
    lv_label_set_text(rpm_sld_name_label,"设置目标速度:");
    lv_obj_align_to(rpm_sld_name_label, view, LV_ALIGN_TOP_LEFT, 0, lv_obj_get_height(ignoreCMDSW)+24);
    
    rpm_sld = lv_slider_create(view);
    lv_slider_set_range(rpm_sld,-60,60);
    lv_obj_set_width(rpm_sld, 130);
    lv_obj_align_to(rpm_sld, rpm_sld_name_label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_add_event_cb(rpm_sld, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    

    /*Create a label below the slider*/
    rpm_sld_value_label = lv_label_create(view);
    lv_label_set_text(rpm_sld_value_label, "");
    lv_obj_align_to(rpm_sld_value_label, rpm_sld, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}


static void lv_spinbox_increment_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(pos_spb);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(pos_spb);
    }
}

static void lv_spinbox_confirm_event_cb(lv_event_t * e){
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED) {
        log_i("pos_spb confirm with value %d",lv_spinbox_get_value(pos_spb));
        if (!setSafe){
            M3508.Angle_Set(lv_spinbox_get_value(pos_spb));
        }
    }
}


static void lv_control_pos_spb(lv_obj_t* view){
    lv_obj_t* pos_spb_name_label = lv_label_create(view);
    lv_obj_set_style_text_font(pos_spb_name_label,p_custom_font,0);
    lv_label_set_text(pos_spb_name_label,"设置目标角度:");
    lv_obj_align_to(pos_spb_name_label, rpm_sld_name_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 12);


    pos_spb = lv_spinbox_create(view);
    lv_spinbox_set_range(pos_spb, -99999, 99999);
    lv_spinbox_set_digit_format(pos_spb, 5, 0);
    lv_spinbox_step_prev(pos_spb);
    lv_obj_set_width(pos_spb, 80);

    lv_coord_t h = lv_obj_get_height(pos_spb);
    lv_obj_align_to(pos_spb, pos_spb_name_label, LV_ALIGN_OUT_BOTTOM_LEFT, h, 4);

    // + button
    lv_obj_t * plus_btn = lv_btn_create(view);
    lv_obj_set_size(plus_btn, h, h);
    lv_obj_align_to(plus_btn, pos_spb, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(plus_btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(plus_btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  nullptr);

    // confirm button
    lv_obj_t * btn = lv_btn_create(view);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, plus_btn, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_OK, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_confirm_event_cb, LV_EVENT_ALL, nullptr);

    // - button
    btn = lv_btn_create(view);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, pos_spb, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, nullptr);
    
}

void lv_control_tab_init(lv_obj_t* view){
    lv_control_style_init();
    lv_control_override_sw(view);
    lv_control_rpm_sld(view);
    lv_control_pos_spb(view);
    lv_control_safe_sw(view);
}
