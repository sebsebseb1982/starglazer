#include "widget-tracking-status.h"
#include "gui.h"
#include "colors.h"
#include "image-satellite.h"

WidgetTrackingStatus::WidgetTrackingStatus(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    TrackedObject *trackedObject,
    TrackingObjectService *trackingObjectService,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
  this->trackingObjectService = trackingObjectService;
  this->trackedObject = trackedObject;
}

void WidgetTrackingStatus::draw()
{
  gui.drawButton(
      x,
      y,
      4,
      2,
      label,
      UNAVAILABLE);

  int textMarginX = 15;
  int textMarginY = 67;
  screen->setTextFont(2);

  String altitude;
  altitude += F("Altitude: ");
  altitude += String(trackingObjectService->currentEquatorialCoordinates.altitude, 1);
  screen->setCursor(x + textMarginX, y + textMarginY);
  screen->print(altitude);

  String azimuth;
  azimuth += F("Azimuth: ");
  azimuth += String(trackingObjectService->currentEquatorialCoordinates.azimuth, 1);
  screen->setCursor(x + textMarginX, y + textMarginY + 20);
  screen->print(azimuth);

  String distance;
  distance += F("Distance: ");
  distance += String(trackingObjectService->currentEquatorialCoordinates.distance / 1000000, 0);
  distance += F(" x 10^6 km");
  screen->setCursor(x + textMarginX, y + textMarginY + 40);
  screen->print(distance);

  previousEquatorialCoordinates = trackingObjectService->currentEquatorialCoordinates;
}

void WidgetTrackingStatus::refreshValue()
{
}

boolean WidgetTrackingStatus::isValueChanged()
{
  int previousEquatorialCoordinatesAzimuth = round(previousEquatorialCoordinates.azimuth * 10);
  int previousEquatorialCoordinatesAltitude = round(previousEquatorialCoordinates.altitude * 10);
  int previousEquatorialCoordinatesDistance = previousEquatorialCoordinates.distance / 1000000;

  int currentEquatorialCoordinatesAzimuth = round(trackingObjectService->currentEquatorialCoordinates.azimuth * 10);
  int currentEquatorialCoordinatesAltitude = round(trackingObjectService->currentEquatorialCoordinates.altitude * 10);
  int currentEquatorialCoordinatesDistance = trackingObjectService->currentEquatorialCoordinates.distance / 1000000;

  boolean isEquatorialCoordinatesChanged = 
    previousEquatorialCoordinatesAzimuth != currentEquatorialCoordinatesAzimuth 
    || previousEquatorialCoordinatesAltitude != currentEquatorialCoordinatesAltitude 
    || previousEquatorialCoordinatesDistance != currentEquatorialCoordinatesDistance;

  return isEquatorialCoordinatesChanged;
}

void WidgetTrackingStatus::manageTouch()
{
  // Serial.println("WidgetTrackingStatus::manageTouch()");
}

void WidgetTrackingStatus::manageTouchDown()
{
  // Serial.println("WidgetTrackingStatus::manageTouchDown()");
}

void WidgetTrackingStatus::manageTouchUp()
{
  // Serial.println("WidgetTrackingStatus::manageTouchUp()");
}
