#include <lvgl.h>
#include <TFT_eSPI.h>
#include "SPIFFS.h"
#include <stdio.h>

#include "gui/gui.h"
lv_style_t my_style; 
static const char * TAG = "Main";
void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    Serial.println();
    SPIFFS.begin();

    guiSetUp();
    
    
}

void loop()
{
    lv_timer_handler();
    delay( 5 );
}
