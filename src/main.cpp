#include <Arduino.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ESPDash.h>

#include "mqtt.h"

#include "pidCardSet.h"
#include "motorCardSet.h"

#include "can.h"
#include "rm_can_motors.h"
static const char *TAG = "MAIN";
/* Your WiFi Credentials */
const char *ssid = "btw_i_use_arch"; // SSID
const char *password = "azxcvbnm";   // Password

extern int16_t output;
extern int16_t rxhz;
extern int16_t txhz;
extern canMotors::motor M3508;
// char cpuStatus[200];
/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server);

pid motorPID(1, 0, 0, 0, 100);
pidCardSet inPIDSet(&dashboard, M3508.getInPID());
motorCardSet mainMotorSet(&dashboard, &M3508);

void setup()
{
  Serial.begin(115200);
  // esp_log_level_set("*", ESP_LOG_WARN);
  ESP_LOGW(TAG, "Running setup");
  
  if (can_init() != ESP_OK)
  {
    ESP_LOGE(TAG, "CAN init failed!");
  }
  motorPID.attach(inPIDSet);

  /* Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    ESP_LOGE(TAG, "WiFi Failed!\n");
    return;
  }
  startMQTT();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /* Start AsyncWebServer */
  server.begin();
  // M3508.Speed_Set(15000);
  // M3508.Angle_Set(1500);
}

void loop()
{
  /* Update Card Values */
  mainMotorSet.update();

  /* Send Updates to our Dashboard (realtime) */
  dashboard.sendUpdates(); // Dont send too fast or get "ERROR: Too many messages queued". 10Hz is surely enough

  /*
    Delay is just for demonstration purposes in this example,
    Replace this code with 'millis interval' in your final project.
  */
  ESP_LOGI("CAN","RealSpd = %d, RXHz = %d, TXHz = %d, SoftAngle = %f",M3508.RealSpeed,rxhz,txhz,M3508.SoftAngle);
  // vTaskList(cpuStatus);
  // Serial.println(cpuStatus);
  // ESP_LOGE("CAN","PIDOut = %d",output);
  vTaskDelay(300);
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
