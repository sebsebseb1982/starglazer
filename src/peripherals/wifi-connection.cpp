#include <WiFi.h>
#include "wifi-connection.h"
#include "debug.h"
#include "secrets.h"

unsigned int WiFiConnection::nbConnection = 0;
WiFiState WiFiConnection::state = WIFI_STATE_CONNECTING;

void WiFiConnection::setup() {
  DEBUG_PRINTLN("WiFiConnection::setup()");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  state = WIFI_STATE_CONNECTING;

  // Blocking wait for initial connection only — called before any task starts
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    retry++;
    if (retry >= 20) {
      ESP.restart();
    }
  }
  nbConnection++;
  state = WIFI_STATE_CONNECTED;
  DEBUG_PRINT("WiFi connected: ");
  DEBUG_PRINTLN(WiFi.localIP());
}

void WiFiConnection::loop() {
  switch (state) {
    case WIFI_STATE_CONNECTED:
      if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WiFi disconnected, reconnecting...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        state = WIFI_STATE_CONNECTING;
      }
      break;

    case WIFI_STATE_CONNECTING: {
      static unsigned long connectStartMs = 0;
      static int retryCount = 0;
      if (connectStartMs == 0) {
        connectStartMs = millis();
        retryCount = 0;
      }
      if (WiFi.status() == WL_CONNECTED) {
        nbConnection++;
        state = WIFI_STATE_CONNECTED;
        connectStartMs = 0;
        DEBUG_PRINT("WiFi reconnected: ");
        DEBUG_PRINTLN(WiFi.localIP());
      } else {
        unsigned long elapsed = millis() - connectStartMs;
        if (elapsed >= 500) {
          connectStartMs = millis();
          retryCount++;
          if (retryCount >= 20) {
            state = WIFI_STATE_FAILED;
          }
        }
      }
      break;
    }

    case WIFI_STATE_FAILED:
      DEBUG_PRINTLN("WiFi reconnection failed, restarting...");
      vTaskDelay(pdMS_TO_TICKS(500));
      ESP.restart();
      break;
  }
}

boolean WiFiConnection::isConnected() {
  return state == WIFI_STATE_CONNECTED;
}