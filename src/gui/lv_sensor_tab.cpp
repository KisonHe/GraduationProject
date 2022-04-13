#include "lv_sensor_tab.h"
#include "fonts.h"
#include <FreeRTOS.h>
#include "gui.h"

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
TaskHandle_t mpu_read_handle;
static lv_obj_t *mpu_info_label;
static lv_obj_t * gyroimg;
SemaphoreHandle_t update_sensor_tab_label = nullptr;
sensors_event_t a, g, temp;

void mpu_read_task(void *n)
{
    while (1)
    {
        mpu.getEvent(&a, &g, &temp);
        vTaskDelay(300);
    }
}

static void lv_sensor_info_label(lv_obj_t* view){
    mpu_info_label = lv_label_create(view);
    lv_label_set_text(mpu_info_label, "");
    lv_obj_set_align(mpu_info_label,LV_ALIGN_BOTTOM_LEFT);
}

static void pxUpdateSensorLabel(TimerHandle_t xTimer){
        xSemaphoreGive(update_sensor_tab_label);
}

void lv_sensor_tab_label_update(){
    lv_label_set_text_fmt(mpu_info_label, "SensorType:MPU6050\nAcceleration X:%.2f Y:%.2f Z:%.2f\nTemperature:%.2f degC",\
    a.acceleration.x,a.acceleration.y,a.acceleration.z,temp.temperature);
}

static void lv_sensor_useless_img(lv_obj_t* view){
    gyroimg = lv_img_create(view);
    lv_img_set_src(gyroimg, "S:/spiffs/gyroimg.bin");
    lv_obj_set_align(gyroimg,LV_ALIGN_TOP_MID);
}

void lv_sensor_tab_init(lv_obj_t* view){
    Wire.begin(25,26);
    if (!mpu.begin()) {
        log_e("Failed to find MPU6050 chip");
    }else{
        xTaskCreatePinnedToCore(mpu_read_task, "mpu_read_task", 4096, NULL, 1, &mpu_read_handle, tskNO_AFFINITY);
    }
    lv_sensor_useless_img(view);
    lv_sensor_info_label(view);

    update_sensor_tab_label = xSemaphoreCreateMutex();
    xTimerStart(xTimerCreate(
            "Update Sensor Tab Info Label",
            pdMS_TO_TICKS(1100),
            pdTRUE,
            ( void * ) 0,
            pxUpdateSensorLabel)
        ,pdMS_TO_TICKS(100)); //wait for 100 ms to make sure it won't get called after lv_layout_init
}
