#include "widget-api-status.h"
#include "network-queue.h"
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
                                              1,
                                              1,
                                              label,
                                              screen,
                                              refreshPeriodInMs),
                                       currentAPIStatus(false),
                                       previousAPIStatus(false),
                                       pendingHealthRequest(false)
{
}

void WidgetAPIStatus::draw()
{
  gui.drawButton(
      x,
      y,
      columnSpan,
      rowSpan,
      label,
      UNAVAILABLE);

  int imageSize = 32;
  screen->drawBitmap(
      x + (BUTTON_SIZE - ICON_SIZE) / 2,
      y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
      cog32x32,
      imageSize,
      imageSize,
      this->currentAPIStatus ? GREEN : ORANGE);

  previousAPIStatus = currentAPIStatus;
}

void WidgetAPIStatus::refreshValue()
{
  // Collect result from network task if available
  bool healthy;
  if (NetworkQueue::tryGetHealth(healthy))
  {
    currentAPIStatus = healthy;
    pendingHealthRequest = false;
  }

  // Send a new request only if not already pending
  if (!pendingHealthRequest)
  {
    NetworkQueue::sendHealthRequest();
    pendingHealthRequest = true;
  }
}

boolean WidgetAPIStatus::isValueChanged()
{
  return currentAPIStatus != previousAPIStatus;
}

void WidgetAPIStatus::manageTouchDown()
{
}

void WidgetAPIStatus::manageTouchUp()
{
}

WidgetAPIStatus::~WidgetAPIStatus()
{
}