#include <arduino.h>
#include "gps.h"

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

void GPS::setup()
{
  Serial2.begin(9600, SERIAL_8N1, 0, 4);
  //Serial2.begin(115200, SERIAL_8N1, 0, 4);
}

void GPS::loop()
{
  if (Serial2.available() > 0)
  {
    if (GPS::gps.encode(Serial2.read()))
    {
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
        Serial.println("GPS::loop() E");
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
}