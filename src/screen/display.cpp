#include "display.h"
#include "screen.h"
#include "equatorial-coordinates-service.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

Display::Display(TFT_eSPI *screen)
  : screen(screen), equatorialCoordinatesService() {
}

void Display::setup() {
}

void Display::loop(GPSData gpsData) {
  this->screen->fillScreen(TFT_BLACK);
  this->screen->setCursor(0, 0);
  this->screen->setTextFont(2);
  this->screen->setTextSize(1);
  this->screen->setTextColor(TFT_WHITE);

  String latitude;
  latitude += F("Lat:");
  latitude += String(gpsData.latitudeInDegrees, 7);

  String longitude;
  longitude += F("Lon:");
  longitude += String(gpsData.longitudeInDegrees, 7);

  String elevation;
  elevation += F("Elevation:");
  elevation += String(gpsData.elevation, 7);

  TrackedObject trackedObject = TrackedObject(F("planets-and-moons"), F("sun"));

  EquatorialCoordinates equatorialCoordinates = this->equatorialCoordinatesService.compute(gpsData, trackedObject);

  String altitude;
  altitude += F("Altitude:");
  altitude += String(equatorialCoordinates.altitude, 7);

  String azimuth;
  azimuth += F("Azimuth:");
  azimuth += String(equatorialCoordinates.azimuth, 7);

  String distance;
  distance += F("Distance:");
  distance += String(equatorialCoordinates.distance, 7);

  this->screen->println(latitude);
  this->screen->println(longitude);
  this->screen->println(elevation);
  this->screen->println(altitude);
  this->screen->println(azimuth);
  this->screen->println(distance);
  delay(5000);
}