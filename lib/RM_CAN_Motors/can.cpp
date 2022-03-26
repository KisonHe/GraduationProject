#include "driver/can.h"
#include "FreeRTOS.h"
#include "esp_log.h"
#include "rm_can_motors.h"
#include <Arduino.h>
static const can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();

static const can_filter_config_t f_config = {.acceptance_code = (0),
                                             .acceptance_mask = (uint32_t)(~(0)),
                                             .single_filter = true};
static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)21, (gpio_num_t)22, CAN_MODE_NO_ACK); // TODO:Change pix to setable

pid spdpid(12, 1.0, 0.1, 1500, 16000);
pid pospid(0.1, 0.01, 0, 20, 20000, 0, 200, 300);
canMotors::motorType DJI_3508(8192, 19.203);

canMotors::motor M3508(0x201, &DJI_3508, &spdpid, &pospid);


int16_t output = 0;
int16_t rxhz;
static int16_t count = 0;
void feedback_update_task(void *n)
{
    can_message_t rx_message;
    ESP_LOGI("CAN_Task", "Start Running");
    static uint32_t lasttime = 0;
    count = 0;
    lasttime = millis();
    canMotors::manager_init();

    while (1)
    {
        // debug -----
        count++;
        if (millis()-lasttime>1000){
            rxhz = (count*1000.0)/((float)(millis()-lasttime));
            lasttime = millis();
            count = 0;
        }
        // debug -----
        ESP_ERROR_CHECK(can_receive(&rx_message, portMAX_DELAY));
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

    if (pdPASS == xTaskCreate(feedback_update_task, "can_read_task", 4096, NULL, 10, &fb_handle))
    {
        return ESP_OK;
    }

    return ESP_FAIL;
}
