#include "driver/can.h"
#include "FreeRTOS.h"
#include "esp_log.h"
#include "rm_can_motors.h"
static const can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();

static const can_filter_config_t f_config = {.acceptance_code = (0),
                                             .acceptance_mask = (uint32_t)(~(0)),
                                             .single_filter = true};
static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)22,(gpio_num_t)23,CAN_MODE_NO_ACK);// TODO:Change pix to setable





void feedback_update_task(void* n){
    can_message_t rx_message;
    ESP_LOGI("CAN_Task","Start Running");

    while(1){
        ESP_ERROR_CHECK(can_receive(&rx_message, portMAX_DELAY));
        ESP_LOGI("CAN_Task","Got CAN msg");

        canMotors::update(rx_message);
    }

}



esp_err_t can_init(void){

    static TaskHandle_t fb_handle;

    //init can interface hardware and driver
    ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));

    //start operation
    ESP_ERROR_CHECK(can_start());

    if(pdPASS == xTaskCreate(feedback_update_task,"can_fb_task",4096,NULL,10,&fb_handle)){
        return ESP_OK;
    }

    return ESP_FAIL;
}
