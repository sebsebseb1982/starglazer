#ifndef STARGLAZE_API_H
#define STARGLAZE_API_H
#include <Arduino.h>
#include <HTTPClient.h>

#define HTTP_RETRY 10

class StarGlazeAPI {
private:
  HTTPClient http;

public:
  StarGlazeAPI();
  boolean health();
  void catalog();
};

#endif
