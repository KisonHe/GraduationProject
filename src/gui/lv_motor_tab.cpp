#include "lv_motor_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include <freertos/timers.h>
#include "../globals.h"
#include <rm_can_motors.h>
#include "gui.h"
extern canMotors::motor M3508;
lv_obj_t *motorTabMainCont = nullptr;

static lv_obj_t * rpmMeter;
static lv_obj_t * posMeter;
lv_obj_t * motor_spd_label;
lv_meter_indicator_t * rpmMeterIndic;
lv_meter_indicator_t * posMeterIndic;

static void set_rpm_value(int32_t v){
    lv_meter_set_indicator_value(rpmMeter,rpmMeterIndic, v);
    lv_label_set_text_fmt(motor_spd_label, "%d", v);
}
/**
 * @brief TODO: 改成往主线程发送一个event，所有更新应该都只有一个位置会操作
 * 
 * @param xTimer 
 */
static void pxUpdateMotorTab(TimerHandle_t xTimer){
    if (xSemaphoreTake(lvgl_mutex, 0) == pdTRUE){
        set_rpm_value(M3508.RealSpeed);
        xSemaphoreGive(lvgl_mutex);
    } 
}

static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(posMeter, posMeterIndic, v);
}

void lv_motor_rpm_meter(lv_obj_t* view)
{
    rpmMeter = lv_meter_create(view);
    // lv_obj_center(rpmMeter);
    lv_obj_set_align(rpmMeter,LV_ALIGN_TOP_LEFT);
    lv_obj_set_size(rpmMeter, 120, 120);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(rpmMeter);
    lv_meter_set_scale_ticks(rpmMeter, scale, 21, 1, 5, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(rpmMeter, scale, 4, 2, 8, lv_color_black(), 10);

    lv_meter_set_scale_range(rpmMeter,scale,0,50,270,135);

    /*Add a blue arc to the start*/
    rpmMeterIndic = lv_meter_add_arc(rpmMeter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(rpmMeter, rpmMeterIndic, 0);
    lv_meter_set_indicator_end_value(rpmMeter, rpmMeterIndic, 10);

    /*Make the tick lines blue at the start of the scale*/
    rpmMeterIndic = lv_meter_add_scale_lines(rpmMeter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(rpmMeter, rpmMeterIndic, 0);
    lv_meter_set_indicator_end_value(rpmMeter, rpmMeterIndic, 10);

    /*Add a red arc to the end*/
    rpmMeterIndic = lv_meter_add_arc(rpmMeter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(rpmMeter, rpmMeterIndic, 35);
    lv_meter_set_indicator_end_value(rpmMeter, rpmMeterIndic, 50);

    /*Make the tick lines red at the end of the scale*/
    rpmMeterIndic = lv_meter_add_scale_lines(rpmMeter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(rpmMeter, rpmMeterIndic, 35);
    lv_meter_set_indicator_end_value(rpmMeter, rpmMeterIndic, 50);

    /*Add a needle line indicator*/
    rpmMeterIndic = lv_meter_add_needle_line(rpmMeter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);


    motor_spd_label = lv_label_create(rpmMeter);
    lv_label_set_text(motor_spd_label, "-");
    lv_obj_add_style(motor_spd_label, &s_font_14_blk, 0);

    lv_obj_t * motor_spd_unit_label = lv_label_create(rpmMeter);
    lv_obj_add_style(motor_spd_unit_label, &s_font_10_blk, 0);
    lv_label_set_text(motor_spd_unit_label, "RPM*100");
    lv_obj_align(motor_spd_label, LV_ALIGN_BOTTOM_MID, -20, 2);
    lv_obj_align_to(motor_spd_unit_label, motor_spd_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 8, 2);
    xTimerStart(xTimerCreate(
                    "Update Motor Tab",
                    pdMS_TO_TICKS(40),
                    pdTRUE,
                    ( void * ) 0,
                    pxUpdateMotorTab)
                ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init

    /*Create an animation to set the value*/
    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_exec_cb(&a, set_value);
    // lv_anim_set_var(&a, rpmMeterIndic);
    // lv_anim_set_values(&a, 0, 40);
    // lv_anim_set_time(&a, 2000);
    // lv_anim_set_repeat_delay(&a, 100);
    // lv_anim_set_playback_time(&a, 500);
    // lv_anim_set_playback_delay(&a, 100);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    // lv_anim_start(&a);
}


void lv_motor_pos_meter(lv_obj_t* view)
{
    posMeter = lv_meter_create(view);
    // lv_obj_center(posMeter);
    lv_obj_set_align(posMeter,LV_ALIGN_TOP_RIGHT);
    lv_obj_set_size(posMeter, 120, 120);

    /*Add small scales */
    lv_meter_scale_t * smallScale = lv_meter_add_scale(posMeter);
    lv_meter_set_scale_ticks(posMeter, smallScale, 21, 1, 5, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(posMeter,smallScale,0,360,360,-90);

    /*Add main scales */
    lv_meter_scale_t * mainScale = lv_meter_add_scale(posMeter);
    lv_meter_set_scale_ticks(posMeter, mainScale, 4, 2, 8, lv_color_black());
    lv_meter_set_scale_major_ticks(posMeter, mainScale, 1, 2, 8, lv_color_black(), 10);    /*Every tick is major*/
    lv_meter_set_scale_range(posMeter,mainScale,90,360,270,0);

    /*Add a needle line indicator*/
    posMeterIndic = lv_meter_add_needle_line(posMeter, smallScale, 4, lv_palette_main(LV_PALETTE_GREY), -10);


    // mbps_label = lv_label_create(posMeter);
    // lv_label_set_text(mbps_label, "-");
    // lv_obj_add_style(mbps_label, &s_font_14_blk, 0);

    // lv_obj_t * motor_spd_unit_label = lv_label_create(posMeter);
    // lv_obj_add_style(motor_spd_unit_label, &s_font_10_blk, 0);
    // lv_label_set_text(motor_spd_unit_label, "RPM*100");
    // lv_obj_align(mbps_label, LV_ALIGN_BOTTOM_MID, -20, 2);
    // lv_obj_align_to(motor_spd_unit_label, mbps_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 8, 2);
    // xTimerStart(xTimerCreate(
    //                 "Update Motor Tab",
    //                 pdMS_TO_TICKS(40),
    //                 pdTRUE,
    //                 ( void * ) 0,
    //                 pxUpdateMotorTab)
    //             ,0);

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_var(&a, rpmMeterIndic);
    lv_anim_set_values(&a, 0, 360);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}


void lv_motor_tab_init(lv_obj_t* view){
    // static lv_obj_t * meter3;
    // lv_obj_t * motorTabContainer = lv_obj_create(view);
    // lv_obj_set_size(motorTabContainer, lv_obj_get_width(view), lv_obj_get_height(view));
    lv_motor_rpm_meter(view);
    lv_motor_pos_meter(view);

}