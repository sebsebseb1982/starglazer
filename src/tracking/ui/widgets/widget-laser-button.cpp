#include "widget-laser-button.h"
#include "gui.h"
#include "colors.h"
#include "image-laser.h"
#include "tracking-object-service.h"

WidgetLaserButton::WidgetLaserButton(
  unsigned int column,
  unsigned int row,
  String label,
  TFT_eSPI *screen,
  unsigned long refreshPeriodInMs
) : Widget(
    column,
    row,
    label,
    screen,
    refreshPeriodInMs
  ) {
}

void WidgetLaserButton::draw() {
  gui.drawToggleButton(
    x,
    y,
    label,
    buttonStatus
  );
  screen->drawBitmap(
    x + (BUTTON_SIZE - ICON_SIZE) / 2,
    y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
    laser32x32,
    32,
    32,
    WHITE
  );
  previousValue = value;
}

void WidgetLaserButton::refreshValue() {
  value = TrackingObjectService::isTracking && TrackingObjectService::isLaserPointingWanted;
  if (!isTouched()) {
    buttonStatus = value ? ACTIVATED : DESACTIVATED;
  }
}

boolean WidgetLaserButton::isValueChanged() {
  return value != previousValue;
}

void WidgetLaserButton::manageTouch() {
}

void WidgetLaserButton::manageTouchDown() {
}

void WidgetLaserButton::manageTouchUp() {
  value = !value;
  TrackingObjectService::isLaserPointingWanted=value;
  buttonStatus = value ? ACTIVATED : DESACTIVATED;
  draw();
}
