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

Screen screen(&tft);
CurrentViewService currentViewService = CurrentViewService(&tft);

TaskHandle_t gpsUpdateTask;
TaskHandle_t lowPriorityTask;
TaskHandle_t touchScreenObserveTask;

void gpsUpdateLoop(void* pvParameters) {
  while (1) {
    GPS::loop();
    vTaskDelay(1);
  }
  vTaskDelete(NULL);
}

void lowPriorityLoop(void* pvParameters) {
  /*
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 10000,          // Timeout en millisecondes
    .idle_core_mask = (1 << 1),  // Core 1 (celui utilisé par cette tâche)
    .trigger_panic = false        // Redémarre le CPU si WDT n'est pas réinitialisé
  };
  esp_task_wdt_init(&wdt_config);

  esp_task_wdt_add(NULL);
*/
  while (1) {
    WiFiConnection::loop();
    screen.loop();
    currentViewService.loop();    
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup()");
  Buzzer::setup();
  Buzzer::off();
  TouchScreen::setup(&tft);
  WiFiConnection::setup();
  screen.setup();
  GPS::setup();
  currentViewService.setup();
  

  
  xTaskCreatePinnedToCore(
    gpsUpdateLoop,   
    "gpsUpdateTask", 
    10000,              
    NULL,               
    5,                  
    &gpsUpdateTask,  
    0);                 
/*
  xTaskCreatePinnedToCore(
    lowPriorityLoop,   
    "lowPriorityTask", 
    10000,             
    NULL,              
    5,                 
    &lowPriorityTask,  
    1);                
*/
  Serial.println("Application ready !");
}

void loop() {
      WiFiConnection::loop();
          GPS::loop();
          TouchScreen::loop();
    screen.loop();
    currentViewService.loop();    
}