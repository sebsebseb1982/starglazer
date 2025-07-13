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
}

void GPS::loop()
{
  if (Serial2.available() > 0)
  {
    if (GPS::gps.encode(Serial2.read()))
    {
      Serial.println("GPS::loop() C");
      if (GPS::gps.location.isValid() && GPS::gps.altitude.isValid())
      {
        Serial.print("GPS::loop() D ");
        Serial.println(gps.location.lat());
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
      /*
            Serial.print(F("- speed: "));
            if (GPS::gps.speed.isValid()) {
              Serial.print(GPS::gps.speed.kmph());
              Serial.println(F(" km/h"));
            } else {
              Serial.println(F("INVALID"));
            }

            Serial.print(F("- GPS date&time: "));
            if (GPS::gps.date.isValid() && GPS::gps.time.isValid()) {
              Serial.print(GPS::gps.date.year());
              Serial.print(F("-"));
              Serial.print(GPS::gps.date.month());
              Serial.print(F("-"));
              Serial.print(GPS::gps.date.day());
              Serial.print(F(" "));
              Serial.print(GPS::gps.time.hour());
              Serial.print(F(":"));
              Serial.print(GPS::gps.time.minute());
              Serial.print(F(":"));
              Serial.println(GPS::gps.time.second());
            } else {
              Serial.println(F("INVALID"));
            }

            Serial.println();
            */
    }
  }

  if (millis() > 5000 && GPS::gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}