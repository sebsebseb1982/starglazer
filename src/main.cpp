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
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("setup()");

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

  xTaskCreatePinnedToCore(
      updateLoop,
      "updateTask",
      10000,
      NULL,
      5,
      &gpsUpdateTask,
      0);

  Serial.println("Application ready !");
}

void loop()
{
  GPS::loop();
  WiFiConnection::loop();
}