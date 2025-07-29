#include "widget-api-status.h"
#include "gui.h"
#include "colors.h"
#include "image-cog.h"
#include "wifi-connection.h"

WidgetAPIStatus::WidgetAPIStatus(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
}

void WidgetAPIStatus::draw()
{
  gui.drawButton(
      x,
      y,
      1,
      1,
      label,
      UNAVAILABLE);

  int textMarginX = 15;
  int textMarginY = 67;
  screen->setTextFont(2);

  int imageSize = 32;

  screen->drawBitmap(
    x + (BUTTON_SIZE - ICON_SIZE) / 2,
    y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
      cog32x32,
      imageSize,
      imageSize,
      GREEN);

  previousAPIStatus = currentAPIStatus;
}

void WidgetAPIStatus::refreshValue()
{
  this->currentAPIStatus = WiFiConnection::isConnected;
}

boolean WidgetAPIStatus::isValueChanged()
{
  return currentAPIStatus != previousAPIStatus;
}

void WidgetAPIStatus::manageTouch()
{
}

void WidgetAPIStatus::manageTouchDown()
{
}

void WidgetAPIStatus::manageTouchUp()
{
}
