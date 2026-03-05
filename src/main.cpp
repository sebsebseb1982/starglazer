#include "esp_log.h"
#include "esp_task_wdt.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "screen.h"
#include "gps.h"
#include "buzzer.h"
#include "wifi-connection.h"
#include "current-view-service.h"
#include "touch-screen.h"
#include "joystick.h"
#include "rgb-led.h"
#include "laser.h"
#include "debug.h"
#include "gimbal.h"
#include "network-queue.h"

Screen screen(&tft);
CurrentViewService currentViewService = CurrentViewService(&tft);

TaskHandle_t gpsUpdateTask;
TaskHandle_t lowPriorityTask;
TaskHandle_t touchScreenObserveTask;

void updateLoop(void *pvParameters)
{
  while (1)
  {
    Joystick::loop();
    screen.loop();
    currentViewService.loop();
    TouchScreen::loop();
    Gimbal::loop();  // Motor stepping — called once per UI iteration
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  DEBUG_PRINTLN("setup()");

  pinMode(2, OUTPUT);

  digitalWrite(2, LOW); // Buzzer
  Laser::off();
  RGBLed::setup();
  Joystick::setup();
  TouchScreen::setup(&tft);
  screen.setup();
  currentViewService.setup();
  GPS::setup();
  WiFiConnection::setup();
  NetworkQueue::setup();  // Starts async network task on Core 1

  xTaskCreatePinnedToCore(
      updateLoop,
      "updateTask",
      10000,
      NULL,
      5,
      &gpsUpdateTask,
      0);

  DEBUG_PRINTLN("Application ready !");
}

void loop()
{
  GPS::loop();
  WiFiConnection::loop();
  vTaskDelay(1);  // Yield to allow GPS/WiFi tasks and networkTask to share Core 1
}