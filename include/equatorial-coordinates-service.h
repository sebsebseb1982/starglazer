#ifndef EQUATORIAL_COORDINATES_SERVICE_H
#define EQUATORIAL_COORDINATES_SERVICE_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <gps.h>

class TrackedObject {
private:

public:
  String type;
  String name;
  TrackedObject(
    String type,
    String name);
};

class EquatorialCoordinates {
private:

public:
  float altitude;
  float azimuth;
  float distance;
  EquatorialCoordinates();
  EquatorialCoordinates(
    float altitude,
    float azimuth,
    float distance);
};

class EquatorialCoordinatesService {
private:
  DynamicJsonDocument doc;
  HTTPClient http;
public:
  EquatorialCoordinatesService();
  EquatorialCoordinates compute(GPSData gpsData, TrackedObject trackedObject);
};

#endif
