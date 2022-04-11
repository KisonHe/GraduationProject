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

static const char *TAG = "Main";
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

void setup()
{
    // ESP_IDF_VERSION_MAJOR;
    Serial.begin(115200); /* prepare for possible serial debug */
    Serial.println();
    SPIFFS.begin();
    esp_log_level_set("*",ESP_LOG_WARN);

    if (can_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "CAN init failed!");
    }
    motorPID.attach(inPIDSet);
    guiSetUp();
    /* Connect WiFi */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        ESP_LOGE(TAG, "WiFi Failed!\n");
        return;
    }
    startMQTT();
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    /* Start AsyncWebServer */
    server.begin();
    // M3508.Speed_Set(15000);
    // M3508.Angle_Set(1500);
}
// extern int32_t mark;
void loop()
{
    mainMotorSet.update();

    dashboard.sendUpdates(); // Dont send too fast or get "ERROR: Too many messages queued". 10Hz is surely enough

    vTaskDelay(100);
}
