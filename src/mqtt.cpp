#include "mqtt.h"
#include <AsyncMqttClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "rm_can_motors.h"
#include "motorCardSet.h"
// #include <pid.h>
AsyncMqttClient mqttClient;
TaskHandle_t MQTT_Task_Handle;
extern canMotors::motor M3508;
extern motorCardSet mainMotorSet;
const char test_topic[] = "test";

DynamicJsonDocument doc(1024);
static String sendString;
// void onMqttConnect(bool sessionPresent)
// {
  
// }

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{

}

static void sendHB(){
    doc["pid_out_p"] = M3508.getOutPID()->getArgs(PIDArgType::kP);
    doc["pid_out_i"] = M3508.getOutPID()->getArgs(PIDArgType::kI);
    doc["pid_out_d"] = M3508.getOutPID()->getArgs(PIDArgType::kD);

    doc["pid_in_p"] = M3508.getInPID()->getArgs(PIDArgType::kP);
    doc["pid_in_i"] = M3508.getInPID()->getArgs(PIDArgType::kI);
    doc["pid_in_d"] = M3508.getInPID()->getArgs(PIDArgType::kD);

    doc["rpm"] = M3508.GetRealSpeed();
    doc["angle"] = M3508.GetSoftAngle();
    doc["control"] = mainMotorSet.isPosCtl;
    sendString = "";    //clear sendString

    serializeJson(doc,sendString);
    mqttClient.publish(test_topic, 1, false, sendString.c_str());

}

void MQTT_Task(void *pvParameters)
{
    IPAddress mqttServerIP;
    WiFi.hostByName("h.kisonhe.xyz", mqttServerIP);

    // mqttClient.onConnect(onMqttConnect);
    mqttClient.onMessage(onMqttMessage);
    
    mqttClient.setServer(mqttServerIP, 1883);
    mqttClient.connect();
    while (1)
    {
        // mqttClient.publish(test_topic, 2, false, "Test");
        sendHB();
        vTaskDelay(2000);
    }
    

}

esp_err_t startMQTT(){
    if (xTaskCreatePinnedToCore(MQTT_Task,
        "MQTT_Task",
        4096,
        nullptr,
        3,
        &MQTT_Task_Handle,
        tskNO_AFFINITY) == pdPASS)
        return ESP_OK;
        else 
        return ESP_ERR_INVALID_RESPONSE;
}
