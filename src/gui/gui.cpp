#include "gui.h"

#include <Arduino.h>
#include <esp_log.h>
#include <FreeRTOS.h>
#include <freertos/timers.h>

#include <TFT_eSPI.h>

#include <lvgl.h>
#include "lvgl_fs.h"
#include "fonts.h"
#include "mainTabView.h"
#include "nvs_flash.h"
#include "nvs.h"
// #include "../test.h"
// Defines
#if CUSTOM_FONT_TYPE == 1
LV_FONT_DECLARE(custom_font);
#endif

extern nvs_handle nvs_main_handle;

// static vars
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];
// SemaphoreHandle_t lvgl_mutex = nullptr;

const lv_font_t* p_custom_font;
lv_style_t s_font_10_blk;
lv_style_t s_font_12_blk;
lv_style_t s_font_14_blk;

// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
// 草，TFT_eSPI/TFT_Drivers/ILI9341_Defines.h 里面，#define TFT_WIDTH  240 #define TFT_HEIGHT 320，也就是长宽是按照rotation = 0的情况区分的，所以。。lvgl给的
// example里面screenWidth  = 320; screenHeight = 240;不适用于我们买的这个，你看lvgl里面又弄了rotation=1。。。
// 如果这样初始化，会导致长短边scale互换（不是xy互换！）

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
TimerHandle_t lv_timer = nullptr;

#if LV_USE_LOG != 0
#if LV_LOG_PRINTF == 0
// TODO: Port this to esplog
/* Serial debugging */
static void my_print(const char * buf)
{
    Serial.print(buf);
    Serial.flush();
}
#endif
#endif


static void load_common_style(){
    lv_style_init(&s_font_10_blk);
    lv_style_set_text_font(&s_font_10_blk, &lv_font_montserrat_10);
    lv_style_init(&s_font_12_blk);
    lv_style_set_text_font(&s_font_12_blk, &lv_font_montserrat_12);
    lv_style_init(&s_font_14_blk);
    lv_style_set_text_font(&s_font_14_blk, &lv_font_montserrat_14);
}

/**
 * @brief set p_custom_font
 * 
 */
static void load_custom_font(){
#if CUSTOM_FONT_TYPE == 2
p_custom_font = lv_font_load("S:/spiffs/customfont.bin");
if (p_custom_font!=nullptr){
    log_i("Success find font from SPIFFS");
}else{
    log_e("Error finding font from SPIFFS, set to fallback");
    p_custom_font = LV_FONT_DEFAULT;
}
#elif CUSTOM_FONT_TYPE == 1
p_custom_font = &custom_font;
#else
p_custom_font = LV_FONT_DEFAULT;
#endif
}

/* Display flushing */
static void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
static void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;
    bool touched = tft.getTouch( &touchX, &touchY, 600 );
    if( !touched ){
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = touchX;data->point.y = touchY;
        // Serial.print( "Data x " );Serial.println( touchX );
        // Serial.print( "Data y " );Serial.println( touchY );
    }
}


TaskHandle_t lvgl_Task_Handle;
// int32_t mark = 0;
static void lvgl_task(TimerHandle_t xTimer)
{
    // static uint32_t lastwake = 0;
    while (1)
    {
        // lastwake = millis();
        // tskTestInit(false);
        // tskTestBegin(false);
        if (xSemaphoreTake(update_motor_tab_meter, 0) == pdTRUE){
            lv_motor_tab_meter_update();
        }
        if (xSemaphoreTake(update_motor_tab_label, 0) == pdTRUE){
            lv_motor_tab_label_update();
        }
        if (xSemaphoreTake(update_network_tab_label, 0) == pdTRUE){
            lv_network_tab_label_update();
        }
        if (xSemaphoreTake(update_sensor_tab_label, 0) == pdTRUE){
            lv_sensor_tab_label_update();
        }
        lv_timer_handler(); /* let the GUI do its work */
        // ESP_LOGD("lvglTask","LVGL Run Intervel: %d", millis() - lastwake);
        // tskTestBegin(false);
        // if (tskTestResult(true)!=0){
        //     log_w("Expected Scheduling happend");
        // }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    
}

/**
 * @brief here we init default screen's layout
 * 
 */
static void lv_layout_init(){
    lv_main_tabview_init();
}
void touch_calibrate(uint16_t* calData)
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    log_i("Calibration Done, with calData[5] = %d %d %d %d %d",calData[0],calData[1],calData[2],calData[3],calData[4]);
    for (int i = 0; i < 5; i++){
        char key[] = "calData0";
        key[7]=i+'0';
        esp_err_t err = nvs_set_u16(nvs_main_handle, key, calData[i]);
        if (err != ESP_OK){
            log_e("Unexpected Error while saving %s:%s",key,esp_err_to_name(err));
        }
    }

}

/**
 * @brief set up all gui stuff:
 *        1. Set up tft_espi
 *        2. Setup lvgl, set lvgl fs callbacks
 *        3. Load Cutsom Font then common styles
 *        4. Call lv_layout_init to init all layouts
 *        5. Set a freertos timer to call lv_timer_handler (The default timer task dont have enough
 *           stack to call lv_timer_handler so it is commented out and called in main loop instead)
 * 
 */
void guiSetUp(){
    // lvgl_mutex = xSemaphoreCreateMutex();
    // Begin set tft_espi
    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    // uint16_t calData[5] = { 419, 3476, 352, 3486, 3 };
    uint16_t calData[5] = { 0, 0, 0, 0, 0 };
    bool success = false;
    for (int i = 0; i < 5; i++){
        char key[] = "calData0";
        key[7]=i+'0';
        esp_err_t err = nvs_get_u16(nvs_main_handle, key, &calData[i]);
        if (err != ESP_OK){
            if (err == ESP_ERR_NVS_NOT_FOUND){
                log_e("%s is not found",key);
                success = false;
            }else{
                log_e("Unexpected Error while getting calData:%s",esp_err_to_name(err));
                success = false;
            }
            break;
        }
        success = true;
    }
    if (!success){
        log_w("caldata not found, calibrating now");
        touch_calibrate(calData);
    }
    tft.setTouch( calData );
    // Set tft_espi Done


    // Begin init lvgl
    lv_init();
#if LV_USE_LOG != 0
#if LV_LOG_PRINTF == 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif
#endif
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );
    // Begin init lvgl's file system & load custom font
    lv_fs_init();
    load_custom_font();
    load_common_style();

    // Start init layout
    lv_layout_init();

    xTaskCreatePinnedToCore(lvgl_task,
                            "LVGL FreeRTOS Timer",
                            8192,
                            nullptr,
                            1,
                            &lvgl_Task_Handle,
                            tskNO_AFFINITY);

}

