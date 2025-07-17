#include "widget-tracking-status.h"
#include "gui.h"
#include "colors.h"
#include "image-compass.h"

#define STATUS_TITLE_Y 10
#define STATUS_CHART_Y 40
#define STATUS_VALUE_Y 100

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
  /*
    int textMarginX = 15;
    int textMarginY = 67;
    screen->setTextFont(2);

    String altitude;
    altitude += F("Altitude : ");
    if (trackingObjectService->isTracking)
    {
      altitude += String(trackingObjectService->currentEquatorialCoordinates.altitude, 1);
    }
    else
    {
      altitude += "waiting ...";
    }
    screen->setCursor(x + textMarginX, y + textMarginY);
    screen->print(altitude);

    String azimuth;
    azimuth += F("Azimuth : ");
    if (trackingObjectService->isTracking)
    {
      azimuth += String(trackingObjectService->currentEquatorialCoordinates.azimuth, 1);
    }
    else
    {
      azimuth += "waiting ...";
    }
    screen->setCursor(x + textMarginX, y + textMarginY + 20);
    screen->print(azimuth);

    String distance;
    distance += F("Distance : ");
    if (trackingObjectService->isTracking)
    {
      distance += String(trackingObjectService->currentEquatorialCoordinates.distance / 1000000, 0);
      distance += F(" x 10^6 km");
    }
    else
    {
      distance += "waiting ...";
    }
    screen->setCursor(x + textMarginX, y + textMarginY + 40);
    screen->print(distance);
  */
  int separatorYStart = 30;
  int separatorYStop = 110;

  drawAltitudeStatus(x + 10, y + 10);
  screen->drawLine(
      x + 107,
      y + separatorYStart,
      x + 107,
      y + separatorYStop,
      VERY_LIGHT_GRAY);
  drawAzimuthStatus(x + 130, y + 10);
  screen->drawLine(
      x + 214,
      y + separatorYStart,
      x + 214,
      y + separatorYStop,
      VERY_LIGHT_GRAY);
  drawDistanceStatus(x + 250, y + 10);

  previousEquatorialCoordinates = trackingObjectService->currentEquatorialCoordinates;
}

void WidgetTrackingStatus::drawAzimuthStatus(unsigned int x, unsigned int y)
{
  screen->setCursor(x + 9, y + STATUS_TITLE_Y);
  screen->print("Azimuth");
  drawAzimuthCompass(x + 9, y + STATUS_CHART_Y);

  String value;
  if (trackingObjectService->isTracking)
  {
    value += String(trackingObjectService->currentEquatorialCoordinates.azimuth, 1);
  }
  else
  {
    value += "waiting ...";
  }
  screen->setCursor(x + 20, y + STATUS_VALUE_Y);
  screen->print(value);
}

void WidgetTrackingStatus::drawAltitudeStatus(unsigned int x, unsigned int y)
{
  screen->setCursor(x + 20, y + STATUS_TITLE_Y);
  screen->print("Altitude");
  drawAltitudeChart(x + 35, y + STATUS_CHART_Y);

  String value;
  if (trackingObjectService->isTracking)
  {
    value += String(trackingObjectService->currentEquatorialCoordinates.altitude, 1);
  }
  else
  {
    value += "waiting ...";
  }
  screen->setCursor(x + 20, y + STATUS_VALUE_Y);
  screen->print(value);
}

void WidgetTrackingStatus::drawDistanceStatus(unsigned int x, unsigned int y)
{
}

void WidgetTrackingStatus::drawAzimuthCompass(unsigned int x, unsigned int y)
{
  uint32_t compassColor = VERY_LIGHT_GRAY;
  int compassSize = 48;
  screen->drawBitmap(
      x,
      y,
      compass48x48,
      compassSize,
      compassSize,
      compassColor);

  float angle = (360.0 - trackingObjectService->currentEquatorialCoordinates.azimuth) + 90.0;

  screen->drawLine(
      x + compassSize / 2,
      y + compassSize / 2,
      x + compassSize / 2 + cos(angle * PI / 180.0) * ((compassSize * 1.0) / 3.0),
      y + compassSize / 2 - sin(angle * PI / 180.0) * ((compassSize * 1.0) / 3.0),
      GREEN);
}

void WidgetTrackingStatus::drawAltitudeChart(unsigned int x, unsigned int y)
{
  uint32_t chartColor = VERY_LIGHT_GRAY;
  int compassSize = 48;
  int tickSize = 2;
  int lineSize = 22;
  screen->drawLine(
      x,
      y,
      x,
      y + compassSize,
      chartColor);

  screen->drawLine(
      x - tickSize,
      y,
      x + tickSize,
      y,
      chartColor);

  screen->drawLine(
      x - tickSize,
      y + compassSize,
      x + tickSize,
      y + compassSize,
      chartColor);

  screen->drawLine(
      x,
      y + compassSize / 2,
      x + compassSize / 2,
      y + compassSize / 2,
      chartColor);

  screen->drawLine(
      x + compassSize / 2,
      y + (compassSize / 2) - tickSize,
      x + compassSize / 2,
      y + (compassSize / 2) + tickSize,
      chartColor);

  screen->drawLine(
      x,
      y + compassSize / 2,
      x + cos(trackingObjectService->currentEquatorialCoordinates.altitude * PI / 180.0) * lineSize,
      y + compassSize / 2 - sin(trackingObjectService->currentEquatorialCoordinates.altitude * PI / 180.0) * lineSize,
      GREEN);
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
      previousEquatorialCoordinatesAzimuth != currentEquatorialCoordinatesAzimuth || previousEquatorialCoordinatesAltitude != currentEquatorialCoordinatesAltitude || previousEquatorialCoordinatesDistance != currentEquatorialCoordinatesDistance;

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
