#include "widget-gps-status.h"
#include "gui.h"
#include "colors.h"
#include "image-satellite.h"

WidgetGPSStatus::WidgetGPSStatus(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              2,
                                              2,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
}

void WidgetGPSStatus::draw()
{
  gui.drawButton(
      x,
      y,
      columnSpan,
      rowSpan,
      label,
      UNAVAILABLE);

  int textMarginX = 15;
  int textMarginY = 67;
  screen->setTextFont(2);

  String latitude;
  latitude += F("Latitude : ");
  if (currentGPSData.isValid)
  {
    latitude += String(currentGPSData.latitudeInDegrees, 5);
  }else {
    latitude += "waiting ...";
  }
  screen->setCursor(x + textMarginX, y + textMarginY);
  screen->print(latitude);

  String longitude;
  longitude += F("Longitude : ");
  if (currentGPSData.isValid)
  {
    longitude += String(currentGPSData.longitudeInDegrees, 5);
  } else {
    longitude += "waiting ...";
  }
  screen->setCursor(x + textMarginX, y + textMarginY + 20);
  screen->print(longitude);

  String elevation;
  elevation += F("Elevation : ");
  if (currentGPSData.isValid)
  {
    elevation += String(currentGPSData.elevation, 0);
    elevation += F(" m");
  }else {
    elevation += "waiting ...";
  }
  screen->setCursor(x + textMarginX, y + textMarginY + 40);
  screen->print(elevation);

  int imageSize = 32;

  screen->drawBitmap(
      x + (BUTTON_SIZE - imageSize / 2),
      y + 18,
      satellite32x32,
      imageSize,
      imageSize,
      currentGPSData.isValid ? GREEN : ORANGE);

  previousGPSData = currentGPSData;
}

void WidgetGPSStatus::refreshValue()
{
  this->currentGPSData = GPS::currentData;
}

boolean WidgetGPSStatus::isValueChanged()
{
  boolean isGPSDataChanged = previousGPSData.latitudeInDegrees != currentGPSData.latitudeInDegrees || previousGPSData.longitudeInDegrees != currentGPSData.longitudeInDegrees || previousGPSData.elevation != currentGPSData.elevation || previousGPSData.isValid != currentGPSData.isValid;
  return isGPSDataChanged;
}

void WidgetGPSStatus::manageTouchDown()
{
}

void WidgetGPSStatus::manageTouchUp()
{
}
