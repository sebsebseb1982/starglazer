#include <Arduino.h>
#include "gps.h"
#include "debug.h"
#include "duration.h"
#include "pinout.h"

GPSData::GPSData()
{
}

GPSData::GPSData(float latitudeInDegrees, float longitudeInDegrees, float elevation, boolean isValid)
{
  this->latitudeInDegrees = latitudeInDegrees;
  this->longitudeInDegrees = longitudeInDegrees;
  this->elevation = elevation;
  this->isValid = isValid;
}

TinyGPSPlus GPS::gps;
GPSData GPS::currentData;
unsigned long GPS::startMillis = 0;
SemaphoreHandle_t GPS::dataMutex = nullptr;

void GPS::setup()
{
  Serial2.begin(9600, SERIAL_8N1, GPS_SERIAL_RX_PIN, GPS_SERIAL_TX_PIN);
  GPS::dataMutex = xSemaphoreCreateMutex();

  GPS::currentData = GPSData(
      0,
      0,
      0,
      false);
}

void GPS::loop()
{

  unsigned long currentMillis = millis();

  if (currentMillis - startMillis >= ONE_MINUTE || !GPS::currentData.isValid)
  {
    if (Serial2.available() > 0)
    {
      if (GPS::gps.encode(Serial2.read()))
      {
       // Serial.println("GPS::gps.encode(Serial2.read())");
        if (GPS::gps.location.isValid() && GPS::gps.altitude.isValid())
        {
          GPSData newData(
              gps.location.lat(),
              gps.location.lng(),
              gps.altitude.meters(),
              true);
          if (xSemaphoreTake(GPS::dataMutex, pdMS_TO_TICKS(50)) == pdTRUE)
          {
            GPS::currentData = newData;
            xSemaphoreGive(GPS::dataMutex);
          }
        }
        else
        {
          GPSData newData(0, 0, 0, false);
          if (xSemaphoreTake(GPS::dataMutex, pdMS_TO_TICKS(50)) == pdTRUE)
          {
            GPS::currentData = newData;
            xSemaphoreGive(GPS::dataMutex);
          }
        }
      }
    }
    else {
      yield();
    }

    if (millis() > 5000 && GPS::gps.charsProcessed() < 10)
        DEBUG_PRINTLN(F("No GPS data received: check wiring"));

    startMillis = currentMillis;
  }
}

GPSData GPS::getDataSafe()
{
  if (GPS::dataMutex == nullptr)
    return GPS::currentData;

  GPSData copy;
  if (xSemaphoreTake(GPS::dataMutex, pdMS_TO_TICKS(20)) == pdTRUE)
  {
    copy = GPS::currentData;
    xSemaphoreGive(GPS::dataMutex);
  }
  return copy;
}