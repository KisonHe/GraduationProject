// #include <lvgl.h>
// #include <TFT_eSPI.h>
// #include "gui/fonts.h"
// #include "gui/lvgl_fs.h"
// /*If you want to use the LVGL examples,
//   make sure to install the lv_examples Arduino library
//   and uncomment the following line.
// #include <lv_examples.h>
// */

// /*Change to your screen resolution*/
// static const uint16_t screenWidth  = 320;
// static const uint16_t screenHeight = 240;
// const lv_font_t* p_custom_font;
// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[ screenWidth * 10 ];

// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc )
// {
//     Serial.printf( "%s(%s)@%d->%s\r\n", file, fn_name, line, dsc );
//     Serial.flush();
// }
// #endif

// /* Display flushing */
// void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
// {
//     uint32_t w = ( area->x2 - area->x1 + 1 );
//     uint32_t h = ( area->y2 - area->y1 + 1 );

//     tft.startWrite();
//     tft.setAddrWindow( area->x1, area->y1, w, h );
//     tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
//     tft.endWrite();

//     lv_disp_flush_ready( disp );
// }

// /*Read the touchpad*/
// void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
// {
//     uint16_t touchX, touchY;

//     bool touched = tft.getTouch( &touchX, &touchY, 600 );

//     if( !touched )
//     {
//         data->state = LV_INDEV_STATE_REL;
//     }
//     else
//     {
//         data->state = LV_INDEV_STATE_PR;

//         /*Set the coordinates*/
//         data->point.x = touchX;
//         data->point.y = touchY;

//         Serial.print( "Data x " );
//         Serial.println( touchX );

//         Serial.print( "Data y " );
//         Serial.println( touchY );
//     }
// }

// static void load_custom_font(){
// #if CUSTOM_FONT_TYPE == 2
// p_custom_font = lv_font_load("S:/spiffs/customfont.bin");
// if (p_custom_font!=nullptr){
//     log_i("Success find font from SPIFFS");
// }else{
//     log_e("Error finding font from SPIFFS, set to fallback");
//     p_custom_font = LV_FONT_DEFAULT;
// }
// #elif CUSTOM_FONT_TYPE == 1
// p_custom_font = &custom_font;
// #else
// p_custom_font = LV_FONT_DEFAULT;
// #endif
// }



// void setup()
// {
//     Serial.begin(115200);
//     SPIFFS.begin();

//     String LVGL_Arduino = "你好, 世界! ";
//     LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

//     Serial.println( LVGL_Arduino );
//     Serial.println( "I am LVGL_Arduino" );

//     lv_init();

//     tft.begin();          /* TFT init */
//     tft.setRotation( 3 ); /* Landscape orientation, flipped */

//     /*Set the touchscreen calibration data,
//      the actual data for your display can be aquired using
//      the Generic -> Touch_calibrate example from the TFT_eSPI library*/
//     uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
//     tft.setTouch( calData );

//     lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );

//     /*Initialize the display*/
//     static lv_disp_drv_t disp_drv;
//     lv_disp_drv_init( &disp_drv );
//     /*Change the following line to your display resolution*/
//     disp_drv.hor_res = screenWidth;
//     disp_drv.ver_res = screenHeight;
//     disp_drv.flush_cb = my_disp_flush;
//     disp_drv.draw_buf = &draw_buf;
//     lv_disp_drv_register( &disp_drv );

//     /*Initialize the (dummy) input device driver*/
//     static lv_indev_drv_t indev_drv;
//     lv_indev_drv_init( &indev_drv );
//     indev_drv.type = LV_INDEV_TYPE_POINTER;
//     indev_drv.read_cb = my_touchpad_read;
//     lv_indev_drv_register( &indev_drv );

//     lv_fs_init();
//     load_custom_font();

// #if 1
//     /* Create simple label */
//     lv_obj_t *label = lv_label_create( lv_scr_act() );
//     lv_label_set_text( label, LVGL_Arduino.c_str() );
//     lv_obj_set_style_text_font(label,&lv_font_simsun_16_cjk,0);
//     // lv_obj_set_style_text_font(label,p_custom_font,0);
//     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
// #else
//     /* Try an example from the lv_examples Arduino library
//        make sure to include it as written above.
//     lv_example_btn_1();
//    */

//     // uncomment one of these demos
//     // lv_demo_widgets();            // OK
//     // lv_demo_benchmark();          // OK
//     // lv_demo_keypad_encoder();     // works, but I haven't an encoder
//     // lv_demo_music();              // NOK
//     // lv_demo_printer();
//     // lv_demo_stress();             // seems to be OK
// #endif
//     Serial.println( "Setup done" );
// }

// void loop()
// {
//     lv_timer_handler(); /* let the GUI do its work */
//     delay( 5 );
// }
