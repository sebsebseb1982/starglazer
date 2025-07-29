#include <Arduino.h>
#include "gps.h"
#include "duration.h"

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

void GPS::setup()
{
  Serial2.begin(9600, SERIAL_8N1, 0, 4);

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
        Serial.println("GPS::gps.encode(Serial2.read())");
        if (GPS::gps.location.isValid() && GPS::gps.altitude.isValid())
        {
          GPS::currentData = GPSData(
              gps.location.lat(),
              gps.location.lng(),
              gps.altitude.meters(),
              true);
        }
        else
        {
          GPS::currentData = GPSData(
              0,
              0,
              0,
              false);
        }
      }
    }

    if (millis() > 5000 && GPS::gps.charsProcessed() < 10)
      Serial.println(F("No GPS data received: check wiring"));

    startMillis = currentMillis;
  }
}