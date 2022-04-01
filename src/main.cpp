#include <lvgl.h>
#include <TFT_eSPI.h>
#include "SPIFFS.h"
#include <stdio.h>
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/
LV_FONT_DECLARE(my_font);
void foo();
/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print( lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc )
{
    Serial.printf( "%s(%s)@%d->%s\r\n", file, fn_name, line, dsc );
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
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
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY, 600 );

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}

lv_style_t my_style; 

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    // SPIFFS.begin(false,"/");
    SPIFFS.begin();
    // fs::File root = SPIFFS.open("/");
    // fs::File file = root.openNextFile();
  
    // while(file){
    //     Serial.print("FILE: ");
    //     Serial.println(file.name());
    //     file = root.openNextFile();
    // }
    // file.close();
    // fs::File file2 = SPIFFS.open("/prova.txt");
    // if (file2.available()){
    //   Serial.println("Read stdio file:");
    //   while(file2.available()){
    //       Serial.write(file2.read());
    //   }
    // }else{
    //   Serial.println("Read arduino api file fail");
    // }

    // FILE* stdfile = fopen("/spiffs/stdio.txt","w");
    // if (stdfile!=nullptr){
    //   char strbuf[50];
    //   Serial.println("Read stdio file:");
    //   fscanf(stdfile,"%s", &strbuf);
    //   Serial.println(strbuf);
    //   fclose(stdfile);
    // }else{
    //   Serial.println("Read stdio file fail");
    // }

    // String LVGL_Arduino = "Hello Arduino! ";
    // LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    String LVGL_Arduino = String("abcdef123456我是中国人");
    Serial.println( LVGL_Arduino );
    // Serial.println( "I am LVGL_Arduino" );

    lv_init();

// #if LV_USE_LOG != 0
//     lv_log_register_print_cb( my_print ); /* register print function for debugging */
// #endif

    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */

    /*Set the touchscreen calibration data,
     the actual data for your display can be aquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
    tft.setTouch( calData );

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

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    /* Create simple label */
    // foo();
    // lv_font_t* my_font = lv_font_load("S:/spiffs/font.bin");
    // lv_style_t my_style;
    // lv_obj_t *label = lv_label_create( lv_scr_act() );
    // if (my_font!=nullptr){
    //   Serial.println("Success find font");
    //   lv_style_set_text_font(&my_style, my_font);
    //   lv_obj_add_style(label, &my_style, _LV_STYLE_STATE_CMP_SAME);
    // }

    
    lv_style_init(&my_style);
    lv_obj_t *label = lv_label_create( lv_scr_act() );

    lv_obj_set_style_text_color(label,lv_palette_main(LV_PALETTE_RED),LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_local_text_color( label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE );
    // lv_obj_set_style_local_bg_color( bkgrnd, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK );

    lv_style_set_text_font(&my_style, &my_font);
    lv_obj_add_style(label, &my_style, LV_PART_MAIN | LV_STATE_DEFAULT);

    
    // LV_LABEL_PART_MAIN
    
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    Serial.println( "Setup done" );
    
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}
