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

// Defines
#if CUSTOM_FONT_TYPE == 1
LV_FONT_DECLARE(custom_font);
#endif

// static vars
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];


const lv_font_t* p_custom_font;
lv_style_t s_font_10_blk;
lv_style_t s_font_12_blk;
lv_style_t s_font_14_blk;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
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
    ESP_LOGI("loadfont","Success find font from SPIFFS");
}else{
    ESP_LOGE("loadfont","Error finding font from SPIFFS, set to fallback");
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



// static void pxCallbackFunction(TimerHandle_t xTimer)
// {
//     lv_timer_handler(); /* let the GUI do its work */
// }

/**
 * @brief here we init default screen's layout
 * 
 */
static void lv_layout_init(){
    lv_main_tabview_init();
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
    // Begin set tft_espi
    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */

    /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
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

    // Start Timer
    // xTimerStart(xTimerCreate(
    //                 "LVGL FreeRTOS Timer",
    //                 pdMS_TO_TICKS(5),
    //                 pdTRUE,
    //                 ( void * ) 0,
    //                 pxCallbackFunction)
    //             ,0);
}
