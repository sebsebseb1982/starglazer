#include "widget-wifi-status.h"
#include "gui.h"
#include "colors.h"
#include "image-wifi.h"
#include "wifi-connection.h"

WidgetWifiStatus::WidgetWifiStatus(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
}

void WidgetWifiStatus::draw()
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

  int imageSize = 32;

  screen->drawBitmap(
    x + (BUTTON_SIZE - ICON_SIZE) / 2,
    y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
      wifi32x32,
      imageSize,
      imageSize,
      currentWifiStatus ? GREEN : ORANGE);

  previousWifiStatus = currentWifiStatus;
}

void WidgetWifiStatus::refreshValue()
{
  this->currentWifiStatus = WiFiConnection::isConnected;
}

boolean WidgetWifiStatus::isValueChanged()
{
  return currentWifiStatus != previousWifiStatus;
}

void WidgetWifiStatus::manageTouchDown()
{
}

void WidgetWifiStatus::manageTouchUp()
{
}
