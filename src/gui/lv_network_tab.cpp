#include "lv_network_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include <freertos/timers.h>
#include "gui.h"
#include "WiFi.h"
#include <Arduino.h>
SemaphoreHandle_t update_network_tab_label = nullptr;
static lv_obj_t *net_info_label;
static lv_obj_t * wifiimg;
static void lv_network_info_label(lv_obj_t* view){
    net_info_label = lv_label_create(view);
    lv_label_set_text(net_info_label, "");
    lv_obj_set_align(net_info_label,LV_ALIGN_BOTTOM_LEFT);
}

static void pxUpdateNetworkLabel(TimerHandle_t xTimer){
        xSemaphoreGive(update_network_tab_label);
}

void lv_network_tab_label_update(){
    lv_label_set_text_fmt(net_info_label, "WiFiName %s:%s\nBSSID:%s\nRSSI:%d\nIP:%s",LV_SYMBOL_WIFI,WiFi.SSID().c_str(),WiFi.BSSIDstr().c_str(),WiFi.RSSI(),WiFi.localIP().toString().c_str());
}

static void lv_network_useless_img(lv_obj_t* view){
    wifiimg = lv_img_create(view);
    lv_img_set_src(wifiimg, "S:/spiffs/wifi120.bin");
    lv_obj_set_align(wifiimg,LV_ALIGN_TOP_MID);
}

static void calib_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        uint16_t calData[5] = { 0, 0, 0, 0, 0 };
        log_w("Start Calib by button");
        touch_calibrate(calData);
        esp_restart();  //Just reboot, it works
    }
}

static void lv_calib_btn(lv_obj_t* view)
{
    lv_obj_t * calib_btn = lv_btn_create(view);
    lv_obj_add_event_cb(calib_btn, calib_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(calib_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    lv_obj_t * label = lv_label_create(calib_btn);
    lv_obj_set_style_text_font(label, p_custom_font, 0);
    lv_label_set_text(label, "触摸校准");
    lv_obj_align_to(label, calib_btn, LV_ALIGN_CENTER, 0, 0);
}

void lv_network_tab_init(lv_obj_t* view){
    lv_network_info_label(view);
    lv_network_useless_img(view);
    lv_calib_btn(view);

    update_network_tab_label = xSemaphoreCreateMutex();
    xTimerStart(xTimerCreate(
            "Update Network Tab Info Label",
            pdMS_TO_TICKS(1100),
            pdTRUE,
            ( void * ) 0,
            pxUpdateNetworkLabel)
        ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init
}
