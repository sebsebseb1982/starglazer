#include <arduino.h>
#include "gps.h"

GPSData::GPSData() {
}

GPSData::GPSData(float latitudeInDegrees, float longitudeInDegrees, float elevation) {
  this->latitudeInDegrees = latitudeInDegrees;
  this->longitudeInDegrees = longitudeInDegrees;
  this->elevation = elevation;
}

float GPSData::getLatitudeInDegrees() {
  return this->latitudeInDegrees;
}

float GPSData::getLongitudeInDegrees() {
  return this->longitudeInDegrees;
}

float GPSData::getLatitudeInRadians() {
  return this->latitudeInDegrees * PI / 180.0;
}

float GPSData::getLongitudeInRadians() {
  return this->longitudeInDegrees * PI / 180.0;
}

float GPSData::getElevation() {
  return this->elevation;
}

GPS::GPS() {
  this->ready = false;
}

void GPS::setup() {
  Serial2.begin(9600, SERIAL_8N1, 0, 4);
}

void GPS::loop() {
  if (Serial2.available() > 0) {
    if (this->gps.encode(Serial2.read())) {
      if (this->gps.location.isValid() && this->gps.altitude.isValid()) {
        this->ready = true;
        this->currentData = GPSData(
          gps.location.lat(),
          gps.location.lng(),
          gps.altitude.meters());
      } else {
        //Serial.println(F("- location: INVALID"));
      }
/*
      Serial.print(F("- speed: "));
      if (this->gps.speed.isValid()) {
        Serial.print(this->gps.speed.kmph());
        Serial.println(F(" km/h"));
      } else {
        Serial.println(F("INVALID"));
      }

      Serial.print(F("- GPS date&time: "));
      if (this->gps.date.isValid() && this->gps.time.isValid()) {
        Serial.print(this->gps.date.year());
        Serial.print(F("-"));
        Serial.print(this->gps.date.month());
        Serial.print(F("-"));
        Serial.print(this->gps.date.day());
        Serial.print(F(" "));
        Serial.print(this->gps.time.hour());
        Serial.print(F(":"));
        Serial.print(this->gps.time.minute());
        Serial.print(F(":"));
        Serial.println(this->gps.time.second());
      } else {
        Serial.println(F("INVALID"));
      }

      Serial.println();
      */
    }
  }

  if (millis() > 5000 && this->gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}