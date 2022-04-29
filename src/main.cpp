#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPDash.h>

#include "mqtt.h"

#include "pidCardSet.h"
#include "motorCardSet.h"

#include "can.h"
#include "rm_can_motors.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "SPIFFS.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <stdio.h>

#include "gui/gui.h"

const char *ssid = "btw_i_use_arch";    // SSID
const char *password = "azxcvbnm"; // Password
extern int16_t output;
extern int16_t rxhz;
extern int16_t txhz;
extern canMotors::motor M3508;
// char cpuStatus[200];
nvs_handle nvs_main_handle;

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

pid motorPID(1, 0, 0, 0, 100);
pidCardSet inPIDSet(&dashboard, M3508.getInPID());
motorCardSet mainMotorSet(&dashboard, &M3508);
unsigned long manager_exe_time = 0;
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
    log_e("Disconnected from WiFi access point\nWiFi lost connection. Reason: %d\nTrying to Reconnect",info.disconnected.reason);
    WiFi.begin(ssid, password);
    stopMQTT();
    server.end();
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
    log_w("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
    log_w("Got IP address: %s",WiFi.localIP().toString().c_str());
    startMQTT();
    server.begin();
}

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println();
    SPIFFS.begin();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        log_w("NVS partition was truncated and needs to be erased,erasing now");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    err = nvs_open("storage", NVS_READWRITE, &nvs_main_handle);
    if (err != ESP_OK){
        log_e("Unexpected Error at nvs_open:%s",esp_err_to_name(err));
    }

    if (can_init() != ESP_OK){
        log_e("CAN init failed!");
    }
    motorPID.attach(inPIDSet);
    guiSetUp();
    /* Connect WiFi */
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
    WiFi.begin(ssid, password);
    vTaskDelay(1000);
    
}
// extern int32_t mark;
void loop()
{
    // static int cnt=0;
    // cnt++;
    if (WiFi.isConnected()){
        mainMotorSet.update();
        dashboard.sendUpdates(); // Dont send too fast or get "ERROR: Too many messages queued". 10Hz is more than surely enough
    }
    // if (cnt>3||(rxhz<900||txhz<900)){
    //     cnt=0;
    //     log_w("RXHz = %d, TXHz = %d, manager run spent %ld",rxhz,txhz,manager_exe_time);
    //     can_status_info_t tmp_info;
    //     can_get_status_info(&tmp_info);
    //     log_w("state:%d msgs_to_tx:%d msgs_to_rx:%d rx_missed_count:%d bus_error_count:%d tx_error_counter:%d rx_error_counter:%d",\
    //     tmp_info.state,tmp_info.msgs_to_tx,tmp_info.msgs_to_rx,tmp_info.rx_missed_count,tmp_info.bus_error_count,tmp_info.bus_error_count,tmp_info.tx_error_counter,tmp_info.rx_error_counter);
    //     // rxhz = 0;txhz = 0;
    // }
        
    vTaskDelay(300);
}
