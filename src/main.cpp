#include "esp_log.h"
#include "esp_task_wdt.h"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "screen.h"
#include "gps.h"
#include "display.h"
#include "buzzer.h"
#include "wifi-connection.h"
#include "current-view-service.h"

Screen screen(&tft);
Display display(&tft);
CurrentViewService currentViewService = CurrentViewService(&tft);

TaskHandle_t highPriorityTask;
TaskHandle_t lowPriorityTask;

void highPriorityLoop(void* pvParameters) {
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
    //display.loop(gps.currentData);

    //  esp_task_wdt_reset();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup()");
  Buzzer::setup();
  Buzzer::off();
  WiFiConnection::setup();
  screen.setup();
  GPS::setup();
  currentViewService.setup();
  //display.setup();

  xTaskCreatePinnedToCore(
    highPriorityLoop,   /* Task function. */
    "highPriorityTask", /* name of task. */
    10000,              /* Stack size of task */
    NULL,               /* parameter of the task */
    2,                  /* priority of the task */
    &highPriorityTask,  /* Task handle to keep track of created task */
    0);                 /* pin task to core 0 */

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
    lowPriorityLoop,   /* Task function. */
    "lowPriorityTask", /* name of task. */
    10000,             /* Stack size of task */
    NULL,              /* parameter of the task */
    1,                 /* priority of the task */
    &lowPriorityTask,  /* Task handle to keep track of created task */
    1);                /* pin task to core 1 */
}

void loop() {
}