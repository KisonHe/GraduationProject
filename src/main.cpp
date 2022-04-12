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
#include <stdio.h>

#include "gui/gui.h"

const char *ssid = "btw_i_use_arch";    // SSID
const char *password = "azxcvbnm"; // Password
extern int16_t output;
extern int16_t rxhz;
extern int16_t txhz;
extern canMotors::motor M3508;
// char cpuStatus[200];
/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

pid motorPID(1, 0, 0, 0, 100);
pidCardSet inPIDSet(&dashboard, M3508.getInPID());
motorCardSet mainMotorSet(&dashboard, &M3508);

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
    // ESP_IDF_VERSION_MAJOR;
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println();
    SPIFFS.begin();

    if (can_init() != ESP_OK)
    {
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
    
}
// extern int32_t mark;
void loop()
{
    if (WiFi.isConnected()){
        mainMotorSet.update();
        dashboard.sendUpdates(); // Dont send too fast or get "ERROR: Too many messages queued". 10Hz is more than surely enough
    }
    vTaskDelay(300);
}
