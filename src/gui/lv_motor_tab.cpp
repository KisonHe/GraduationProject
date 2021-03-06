// #include <Arduino.h>
#include "lv_motor_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include <freertos/timers.h>
#include "../globals.h"
#include <rm_can_motors.h>
#include "gui.h"
#include "../globals.h"

extern canMotors::motor M3508;

static lv_obj_t *rpmMeter;
static lv_obj_t *posMeter;
static lv_obj_t *motor_spd_label;
static lv_obj_t *pos_label;
static lv_obj_t *pos_unit_label;
static lv_obj_t *info_label;
static lv_meter_indicator_t * rpmMeterIndic;
static lv_meter_indicator_t * posMeterIndic;

SemaphoreHandle_t update_motor_tab_meter;
SemaphoreHandle_t update_motor_tab_label;

static void set_rpm_value(int32_t v){
    lv_meter_set_indicator_value(rpmMeter,rpmMeterIndic, v/10);
    lv_label_set_text_fmt(motor_spd_label, "%.1f", v/10.0);
}

static void set_pos_value(float v){
    // int16_t tmp = v>0?((int16_t)v)%360:(360-abs((int16_t)v)%360);
    lv_meter_set_indicator_value(posMeter,posMeterIndic, (int16_t)v>0?((int16_t)v)%360:(360-abs((int16_t)v)%360));
    lv_label_set_text_fmt(pos_label, "%d", ((int16_t)v)%360);
}

static void pxUpdateMotorMeter(TimerHandle_t xTimer){
        xSemaphoreGive(update_motor_tab_meter);
}

static void pxUpdateMotorLabel(TimerHandle_t xTimer){
        xSemaphoreGive(update_motor_tab_label);
}

static void lv_motor_rpm_meter(lv_obj_t* view){
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

    lv_obj_clear_flag(rpmMeter, LV_OBJ_FLAG_SCROLLABLE);
}

static void lv_motor_pos_meter(lv_obj_t* view){
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


    pos_label = lv_label_create(posMeter);
    lv_label_set_text(pos_label, "-");
    lv_obj_add_style(pos_label, &s_font_14_blk, 0);

    lv_obj_t * pos_unit_label = lv_label_create(posMeter);
    lv_obj_add_style(pos_unit_label, &s_font_10_blk, 0);
    lv_label_set_text(pos_unit_label, "??");
    lv_obj_set_style_text_font(pos_unit_label,p_custom_font,0);
    lv_obj_align(pos_label, LV_ALIGN_BOTTOM_MID, 0, 16);
    lv_obj_align_to(pos_unit_label, pos_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);

}

void lv_motor_tab_label_update(){
    lv_label_set_text_fmt(info_label, "??????P:%.1f I:%.1f D:%.1f\n??????P:%.1f I:%.1f D:%.1f\n????????????:%d ??????????????????%.3f\n?????????:%s ????????????:%d\n????????????:%.3f",\
                                        M3508.getInPID()->getArgs(PIDArgType::kP),
                                        M3508.getInPID()->getArgs(PIDArgType::kI),
                                        M3508.getInPID()->getArgs(PIDArgType::kD),
                                        M3508.getOutPID()->getArgs(PIDArgType::kP),
                                        M3508.getOutPID()->getArgs(PIDArgType::kI),
                                        M3508.getOutPID()->getArgs(PIDArgType::kD),
                                        M3508.GetRealSpeed(),M3508.GetSoftAngle(),
                                        M3508.RunState == canMotors::RunState_t::Stop ? "??????" : M3508.RunState == canMotors::RunState_t::Speed_Ctl ? "????????????" : "????????????",
                                        M3508.TargetSpeed,
                                        M3508.Target_Angle
                                        );

}




#ifdef DEBUG_PID
lv_obj_t* debug_pid_label = nullptr;
void lv_motor_tab_meter_update(){
    // lv_label_set_text_fmt(debug_pid_label,"SPD:\nIOut:% 4.1f PIDOut:% 4.1f\nCE:% 4.1f LE:% 4.1f",\
    //                         M3508.getInPID()->Iout,
    //                         M3508.getInPID()->PIDout,
    //                         M3508.getInPID()->CurrentError,
    //                         M3508.getInPID()->LastError
    //                         );
    printf("SPD:> IOut:%6d PIDOut:%6d ????????????:%6d ????????????:%6d ????????????:%6d \n",\
                            (int)M3508.getInPID()->Iout,
                            (int)M3508.getInPID()->PIDout,
                            (int)M3508.getInPID()->CurrentError,
                            (int)M3508.getInPID()->LastError,
                            M3508.RealSpeed
    );
}


static void lv_motor_debugpid_label(lv_obj_t* view){
    debug_pid_label = lv_label_create(view);
    lv_label_set_text(debug_pid_label, "");
}

static void lv_motor_info_label(lv_obj_t* view){
    info_label = lv_label_create(view);
    lv_label_set_text(info_label, "");
    lv_obj_set_style_text_font(info_label,p_custom_font,0);
    lv_obj_set_align(info_label,LV_ALIGN_BOTTOM_LEFT);
}


void lv_motor_tab_init(lv_obj_t* view){
    lv_motor_debugpid_label(view);
    lv_motor_info_label(view);
    update_motor_tab_label = xSemaphoreCreateMutex();
    update_motor_tab_meter = xSemaphoreCreateMutex();
    xTimerStart(xTimerCreate(
                "Update PID Label",
                pdMS_TO_TICKS(50),
                pdTRUE,
                ( void * ) 0,
                pxUpdateMotorMeter)
                ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init
    xTimerStart(xTimerCreate(
                "Update Motor Tab",
                pdMS_TO_TICKS(1100),
                pdTRUE,
                ( void * ) 0,
                pxUpdateMotorLabel)
            ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init

}
#else

static void lv_motor_info_label(lv_obj_t* view){
    info_label = lv_label_create(view);
    lv_label_set_text(info_label, "");
    lv_obj_set_style_text_font(info_label,p_custom_font,0);
    lv_obj_align_to(info_label, rpmMeter, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
}

void lv_motor_tab_meter_update(){
    static int cnt = 0;
    if (cnt++%10 == 0){
        set_rpm_value(abs(M3508.RealSpeed)/10); //??????????????????????????????????????????????????????????????????
    }
    set_pos_value(M3508.GetSoftAngle());
}

void lv_motor_tab_init(lv_obj_t* view){
    // static lv_obj_t * meter3;
    // lv_obj_t * motorTabContainer = lv_obj_create(view);
    // lv_obj_set_size(motorTabContainer, lv_obj_get_width(view), lv_obj_get_height(view));
    lv_motor_rpm_meter(view);
    lv_motor_pos_meter(view);
    lv_motor_info_label(view);
    update_motor_tab_label = xSemaphoreCreateMutex();
    update_motor_tab_meter = xSemaphoreCreateMutex();
    xTimerStart(xTimerCreate(
                "Update Motor Meters",
                pdMS_TO_TICKS(15),
                pdTRUE,
                ( void * ) 0,
                pxUpdateMotorMeter)
                ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init
    xTimerStart(xTimerCreate(
                "Update Motor Tab",
                pdMS_TO_TICKS(1100),
                pdTRUE,
                ( void * ) 0,
                pxUpdateMotorLabel)
            ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init

}
#endif