#ifndef STARGLAZE_API_H
#define STARGLAZE_API_H
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "catalog.h"

#define HTTP_RETRY 10

class StarGlazeAPI {
private:
  HTTPClient http;
  JsonDocument doc;

public:
  StarGlazeAPI();
  boolean health();
  Catalog getCatalog();
};

#endif
