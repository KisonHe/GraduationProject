#include "driver/can.h"
#include "FreeRTOS.h"
#include "esp_log.h"
#include "rm_can_motors.h"
#include <Arduino.h>
static const can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();

static const can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)21, (gpio_num_t)22, CAN_MODE_NORMAL); // TODO:Change pix to setable

pid spdpid(12, 0, 0, 0, 16000);    // PID库关于I/D的部分很可能有Bug
pid pospid(0.1, 0, 0, 0, 20000);
canMotors::motorType DJI_3508(8192, 19.203);

canMotors::motor M3508(0x201, &DJI_3508, &spdpid, &pospid);


int16_t output = 0;
int16_t rxhz;
static int16_t count = 0;
static const char taskname[] = "can_read_task";//Damn vTaskGetInfo again not compiled fk
void feedback_update_task(void *n)
{
    can_message_t rx_message;
    ESP_LOGI("CAN_Task", "Start Running");
    canMotors::manager_init();
    // TickType_t xLastWakeTime;
    // xLastWakeTime = xTaskGetTickCount();
    // static const TickType_t MaxInterval = 5;
    static uint32_t lasttime = 0;
    count = 0;
    lasttime = millis();
    
    while (1){
        esp_err_t rt = can_receive(&rx_message, pdMS_TO_TICKS(3));
        if(rt!=ESP_OK){
            log_e("Error Receiving CAN Msg :%s",esp_err_to_name(rt));
            continue;
        }else{
            count++;
        }
        // debug -----
        
        if (millis()-lasttime>1000){
            rxhz = (count*1000.0)/((float)(millis()-lasttime));
            lasttime = millis();
            count = 0;
        }
        // debug -----
        // if (xTaskGetTickCount() - xLastWakeTime > MaxInterval){
        //     log_w("Task %s run interval longer than expectation:%d",taskname,xTaskGetTickCount() - xLastWakeTime);
        // }
        // xLastWakeTime = xTaskGetTickCount();
        if ((rx_message.identifier) > 0x200 && (rx_message.identifier) < 0x20C && rx_message.data_length_code == 8){
            canMotors::update(rx_message.identifier,rx_message.data);
        }
    }
}

esp_err_t can_init(void)
{

    static TaskHandle_t fb_handle;

    // init can interface hardware and driver
    ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));

    // start operation
    ESP_ERROR_CHECK(can_start());

    // uint32_t alerts_to_enable = CAN_ALERT_AND_LOG | CAN_ALERT_ERR_PASS | CAN_ALERT_BUS_OFF | CAN_ALERT_BUS_ERROR | CAN_ALERT_RX_QUEUE_FULL;

    // if (can_reconfigure_alerts(alerts_to_enable, NULL) == ESP_OK) {
    //     log_i("Alerts reconfigured\n");
    // } else {
    //     log_e("Failed to reconfigure alerts");
    // }

    if (pdPASS == xTaskCreatePinnedToCore(feedback_update_task, taskname, 4096, NULL, 23, &fb_handle, tskNO_AFFINITY))
    {
        return ESP_OK;
    }

    return ESP_FAIL;
}
