#include "widget-tracking-status.h"
#include "gui.h"
#include "colors.h"
#include "image-compass.h"
#include "image-distance.h"

#define STATUS_TITLE_Y 10
#define STATUS_CHART_Y 40
#define STATUS_VALUE_Y 100

WidgetTrackingStatus::WidgetTrackingStatus(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    ObjectToWatch *trackedObject,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              4,
                                              2,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
  this->trackedObject = trackedObject;
}

void WidgetTrackingStatus::draw()
{
  gui.drawButton(
      x,
      y,
      columnSpan,
      rowSpan,
      label,
      UNAVAILABLE);
  
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
  drawDistanceStatus(x + 230, y + 10);

  previousEquatorialCoordinates = TrackingObjectService::currentEquatorialCoordinates;
}

void WidgetTrackingStatus::drawAzimuthStatus(unsigned int x, unsigned int y)
{
  screen->setCursor(x + 9, y + STATUS_TITLE_Y);
  screen->print("Azimuth");
  drawAzimuthCompass(x + 9, y + STATUS_CHART_Y);

  String value;
  if (TrackingObjectService::isTracking)
  {
    value += String(TrackingObjectService::currentEquatorialCoordinates.azimuth, 1);
  }
  else
  {
    value += "waiting ...";
  }
  screen->setCursor(x + 15, y + STATUS_VALUE_Y);
  screen->print(value);
}

void WidgetTrackingStatus::drawAltitudeStatus(unsigned int x, unsigned int y)
{
  screen->setCursor(x + 20, y + STATUS_TITLE_Y);
  screen->print("Altitude");
  drawAltitudeChart(x + 35, y + STATUS_CHART_Y);

  String value;
  if (TrackingObjectService::isTracking)
  {
    value += String(TrackingObjectService::currentEquatorialCoordinates.altitude, 1);
  }
  else
  {
    value += "waiting ...";
  }
  screen->setCursor(x + 24, y + STATUS_VALUE_Y);
  screen->print(value);
}

void WidgetTrackingStatus::drawDistanceStatus(unsigned int x, unsigned int y)
{
  screen->setCursor(x + 14, y + STATUS_TITLE_Y);
  screen->print("Distance");
  drawDistanceChart(x + 14, y + STATUS_CHART_Y);

  String value;
  if (TrackingObjectService::isTracking)
  {
    char buffer[32];
    sprintf(buffer, "%.1e", TrackingObjectService::currentEquatorialCoordinates.distance);
    value += buffer;
    value += " km";
  }
  else
  {
    value += "waiting ...";
  }
  screen->setCursor(x + 5, y + STATUS_VALUE_Y);
  screen->print(value);
}

void WidgetTrackingStatus::drawDistanceChart(unsigned int x, unsigned int y)
{
  uint32_t chartColor = VERY_LIGHT_GRAY;
  int chartSize = 48;
  screen->drawBitmap(
      x,
      y,
      distance48x48,
      chartSize,
      chartSize,
      chartColor);
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

  float angle = (360.0 - TrackingObjectService::currentEquatorialCoordinates.azimuth) + 90.0;

  if (TrackingObjectService::isTracking)
  {
    screen->drawLine(
        x + compassSize / 2,
        y + compassSize / 2,
        x + compassSize / 2 + cos(angle * PI / 180.0) * ((compassSize * 1.0) / 3.0),
        y + compassSize / 2 - sin(angle * PI / 180.0) * ((compassSize * 1.0) / 3.0),
        GREEN);
  }
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

  if (TrackingObjectService::isTracking)
  {
    screen->drawLine(
        x,
        y + compassSize / 2,
        x + cos(TrackingObjectService::currentEquatorialCoordinates.altitude * PI / 180.0) * lineSize,
        y + compassSize / 2 - sin(TrackingObjectService::currentEquatorialCoordinates.altitude * PI / 180.0) * lineSize,
        GREEN);
  }
}

void WidgetTrackingStatus::refreshValue()
{
}

boolean WidgetTrackingStatus::isValueChanged()
{
  int previousEquatorialCoordinatesAzimuth = round(previousEquatorialCoordinates.azimuth * 10);
  int previousEquatorialCoordinatesAltitude = round(previousEquatorialCoordinates.altitude * 10);
  int previousEquatorialCoordinatesDistance = previousEquatorialCoordinates.distance / 1000000;

  int currentEquatorialCoordinatesAzimuth = round(TrackingObjectService::currentEquatorialCoordinates.azimuth * 10);
  int currentEquatorialCoordinatesAltitude = round(TrackingObjectService::currentEquatorialCoordinates.altitude * 10);
  int currentEquatorialCoordinatesDistance = TrackingObjectService::currentEquatorialCoordinates.distance / 1000000;

  boolean isEquatorialCoordinatesChanged =
      previousEquatorialCoordinatesAzimuth != currentEquatorialCoordinatesAzimuth || previousEquatorialCoordinatesAltitude != currentEquatorialCoordinatesAltitude || previousEquatorialCoordinatesDistance != currentEquatorialCoordinatesDistance;

  return isEquatorialCoordinatesChanged;
}

void WidgetTrackingStatus::manageTouchDown()
{
  // Serial.println("WidgetTrackingStatus::manageTouchDown()");
}

void WidgetTrackingStatus::manageTouchUp()
{
  // Serial.println("WidgetTrackingStatus::manageTouchUp()");
}
