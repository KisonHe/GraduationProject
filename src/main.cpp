#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPDash.h>

#include "pidCardSet.h"
#include "pid.h"
/* Your WiFi Credentials */
const char* ssid = "btw_i_use_arch"; // SSID
const char* password = "azxcvbnm"; // Password

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 


pid motorPID(1,0,0,0,100);
pidCardSet mainSet(&dashboard,&motorPID);


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
  dashboard.sendUpdates();

  /* 
    Delay is just for demonstration purposes in this example,
    Replace this code with 'millis interval' in your final project.
  */
  delay(10);
}