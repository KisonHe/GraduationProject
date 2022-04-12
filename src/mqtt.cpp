#include "mqtt.h"
#include <AsyncMqttClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "rm_can_motors.h"
#include "motorCardSet.h"
#define ID "3508"

AsyncMqttClient mqttClient;
TaskHandle_t MQTT_Task_Handle = nullptr;
extern canMotors::motor M3508;
extern motorCardSet mainMotorSet;
// const char test_topic[] = "test";
const char hb_topic[] = "gp/hb/" ID;
const char cmd_topic[] = "gp/cmd/" ID;

// static DynamicJsonDocument doc(1024); // ! NO, NEVER REUSE JsonDocument !

// Why is it wrong to reuse a JsonDocument?
// JsonDocument contains a monotonic allocator: a fast and lightweight allocator that cannot release the memory. Because it cannot free memory, each time you remove or replace a value in a JsonDocument, the old value remains in memory. To release memory, you must either clear or destroy the entire JsonDocument.
// ArduinoJson uses this kind of allocator because it provides the best performance with the smallest code. Most users don’t notice the problem, but you can run into it if you reuse the same JsonDocument without destroying or clearing it.
// REMEMBER that ArduinoJson is a serialization library. As such, it is designed to serialize and deserialize JSON documents, NOT TO STORE the state of an application.

void onMqttConnect(bool sessionPresent)
{
    if (MQTT_Task_Handle!=nullptr)
        vTaskResume(MQTT_Task_Handle);
    else
        log_e("Trying to resume mqttTask with null task handle!");
}

// 哎随便糊个学术与代码垃圾吧
// {
//   "commandType": "set",
//   "pid_out_p": 1,
//   "pid_out_i": 1,
//   "pid_out_d": 1,
//   "pid_in_p": 1,
//   "pid_in_i": 1,
//   "pid_in_d": 1,
//   "control": "spd",
//   "controlValue" : 1000
// }

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    Serial.println("Got mqtt msg");
    if (strcmp(cmd_topic, topic) == 0)
    {
        if (len < 500)
        {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            // String tmp = doc["commandType"];
            // Serial.println(tmp);
            // Serial.println(strcmp(doc["commandType"],"set"));

            if (doc["commandType"] != nullptr && strcmp(doc["commandType"], "set") == 0)
            { //不检查类型了，学术垃圾，能用就行，不多花时间了（
                Serial.println("Got set msg");
                if (doc["pid_out_p"])
                {
                    M3508.getOutPID()->setArgs(PIDArgType::kP, doc["pid_out_p"]);
                }
                if (doc["pid_out_i"])
                {
                    M3508.getOutPID()->setArgs(PIDArgType::kI, doc["pid_out_p"]);
                }
                if (doc["pid_out_d"])
                {
                    M3508.getOutPID()->setArgs(PIDArgType::kD, doc["pid_out_p"]);
                }
                if (doc["pid_out_p"])
                {
                    M3508.getInPID()->setArgs(PIDArgType::kP, doc["pid_in_p"]);
                }
                if (doc["pid_out_i"])
                {
                    M3508.getInPID()->setArgs(PIDArgType::kI, doc["pid_in_p"]);
                }
                if (doc["pid_out_d"])
                {
                    M3508.getInPID()->setArgs(PIDArgType::kD, doc["pid_in_p"]);
                }
                if (doc["control"])
                {
                    if (strcmp(doc["control"], "spd") == 0)
                    {
                        M3508.Speed_Set(doc["controlValue"] | 0);
                        mainMotorSet.isPosCtl = false;
                    }
                    else if (strcmp(doc["control"],"pos"))
                    {
                        M3508.Speed_Set(doc["controlValue"] | M3508.GetSoftAngle());
                        mainMotorSet.isPosCtl = true;
                    }else{
                        ESP_LOGE("mqtt", "Unknown control type");
                    }
                }
            }
        }
    }
}

static void sendHB()
{
    DynamicJsonDocument doc(1024);
    String sendString;
    doc["pid_out_p"] = M3508.getOutPID()->getArgs(PIDArgType::kP);
    doc["pid_out_i"] = M3508.getOutPID()->getArgs(PIDArgType::kI);
    doc["pid_out_d"] = M3508.getOutPID()->getArgs(PIDArgType::kD);

    doc["pid_in_p"] = M3508.getInPID()->getArgs(PIDArgType::kP);
    doc["pid_in_i"] = M3508.getInPID()->getArgs(PIDArgType::kI);
    doc["pid_in_d"] = M3508.getInPID()->getArgs(PIDArgType::kD);

    doc["rpm"] = M3508.GetRealSpeed();
    doc["angle"] = M3508.GetSoftAngle();
    doc["control"] = mainMotorSet.isPosCtl;
    serializeJson(doc, sendString);
    mqttClient.publish(hb_topic, 1, false, sendString.c_str());
}

void MQTT_Task(void *pvParameters)
{
    IPAddress mqttServerIP;
    WiFi.hostByName("h.kisonhe.xyz", mqttServerIP);

    // mqttClient.onDisconnect();
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onMessage(onMqttMessage);

    mqttClient.setServer(mqttServerIP, 1883);
    mqttClient.connect();
    vTaskSuspend(nullptr); // Wait until connect
    mqttClient.subscribe(cmd_topic, 2);
    while (1)
    {
        sendHB();
        vTaskDelay(3000);
    }
}

esp_err_t startMQTT()
{
    if (MQTT_Task_Handle!=nullptr){
        log_e("MQTT_Task_Handle already not nullptr!");
        return ESP_FAIL;
    }
    if (xTaskCreatePinnedToCore(MQTT_Task,
                                "MQTT_Task",
                                8192,
                                nullptr,
                                3,
                                &MQTT_Task_Handle,
                                tskNO_AFFINITY) == pdPASS)
        return ESP_OK;
    else
        return ESP_FAIL;
}

esp_err_t stopMQTT()
{
    if (MQTT_Task_Handle==nullptr){
        log_e("Deleting but MQTT_Task_Handle is nullptr!");
        return ESP_FAIL;
    }
    else{
        vTaskDelete(MQTT_Task_Handle);
        return ESP_OK;
    }
    
    
}

