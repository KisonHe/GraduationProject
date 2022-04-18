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

void lv_network_tab_init(lv_obj_t* view){
    lv_network_info_label(view);
    lv_network_useless_img(view);

    update_network_tab_label = xSemaphoreCreateMutex();
    xTimerStart(xTimerCreate(
            "Update Network Tab Info Label",
            pdMS_TO_TICKS(1100),
            pdTRUE,
            ( void * ) 0,
            pxUpdateNetworkLabel)
        ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init
}
