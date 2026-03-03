#include <WiFi.h>
#include "wifi-connection.h"
#include "debug.h"
#include "secrets.h"

unsigned int WiFiConnection::nbConnection = 0;

void WiFiConnection::setup() {
  DEBUG_PRINTLN("WiFiConnection::setup()");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int retry = 0; 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    retry ++;

    if(retry == 20) {
      ESP.restart();
    }
    //Serial.print(".");
  }
  nbConnection++;
  DEBUG_PRINT("Connecté au WIFI avec l'adresse IP : ");
  DEBUG_PRINTLN(WiFi.localIP());
}

void WiFiConnection::loop() {
  if(WiFi.status() != WL_CONNECTED) {
    setup();
  }
}

boolean WiFiConnection::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}