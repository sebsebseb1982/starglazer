#include "widget-tracking-button.h"
#include "gui.h"
#include "colors.h"
#include "image-target.h"

WidgetTrackingButton::WidgetTrackingButton(
  unsigned int column,
  unsigned int row,
  String label,
  TFT_eSPI *screen,
  unsigned long refreshPeriodInMs
) : Widget(
    column,
    row,
    1,
    1,
    label,
    screen,
    refreshPeriodInMs
  ) {
}

void WidgetTrackingButton::draw() {
  gui.drawToggleButton(
    x,
    y,
    label,
    buttonStatus
  );
  screen->drawBitmap(
    x + (BUTTON_SIZE - ICON_SIZE) / 2,
    y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
    target32x32,
    32,
    32,
    WHITE
  );
  previousValue = value;
}

void WidgetTrackingButton::refreshValue() {
  value = TrackingObjectService::isTracking;
  if (!isTouched()) {
    buttonStatus = value ? ACTIVATED : DESACTIVATED;
  }
}

boolean WidgetTrackingButton::isValueChanged() {
  return value != previousValue;
}

void WidgetTrackingButton::manageTouchDown() {
}

void WidgetTrackingButton::manageTouchUp() {
  value = !value;
  TrackingObjectService::isTracking=value;
  buttonStatus = value ? ACTIVATED : DESACTIVATED;
  draw();
}
