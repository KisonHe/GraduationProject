#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPDash.h>

#include "pidCardSet.h"
#include "motorCardSet.h"
/* Your WiFi Credentials */
const char* ssid = "btw_i_use_arch"; // SSID
const char* password = "azxcvbnm"; // Password

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 


pid motorPID(1,0,0,0,100);
pidCardSet mainSet(&dashboard,&motorPID);
motorCardSet mainMotorSet(&dashboard);

void setup() {
  Serial.begin(115200);
  motorPID.attach(mainSet);


  /* Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  /* Start AsyncWebServer */
  server.begin();
}

void loop() {
  /* Update Card Values */

  /* Send Updates to our Dashboard (realtime) */
  dashboard.sendUpdates();  //Dont send too fast or get "ERROR: Too many messages queued". 10Hz is surely enough

  /* 
    Delay is just for demonstration purposes in this example,
    Replace this code with 'millis interval' in your final project.
  */
  vTaskDelay(100);
}


// BTW These doesn't work on esp dash ordering
// Number    Name                   HTML Code    Appearance
// ------    --------------------   ---------    ----------
// U+2000    En Quad                &#8192;      " "
// U+2001    Em Quad                &#8193;      " "
// U+2002    En Space               &#8194;      " "
// U+2003    Em Space               &#8195;      " "
// U+2004    Three-Per-Em Space     &#8196;      " "
// U+2005    Four-Per-Em Space      &#8197;      " "
// U+2006    Six-Per-Em Space       &#8198;      " "
// U+2007    Figure Space           &#8199;      " "
// U+2008    Punctuation Space      &#8200;      " "
// U+2009    Thin Space             &#8201;      " "
// U+200A    Hair Space             &#8202;      " "
// U+200B    Zero-Width Space       &#8203;      "​"
// U+200C    Zero Width Non-Joiner  &#8204;      "‌"
// U+200D    Zero Width Joiner      &#8205;      "‍"
