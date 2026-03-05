#include "widget-recalibrate-button.h"
#include "gui.h"
#include "colors.h"
#include "image-cog.h"
#include "current-view-service.h"
#include "calibration-view.h"
#include "tracking-object-service.h"

WidgetRecalibrateButton::WidgetRecalibrateButton(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    ObjectToWatch *trackedObject,
    unsigned long refreshPeriodInMs) : Widget(
                                           column,
                                           row,
                                           1,
                                           1,
                                           label,
                                           screen,
                                           refreshPeriodInMs),
                                       trackedObject(trackedObject)
{
}

void WidgetRecalibrateButton::draw()
{
    gui.drawButton(
        x,
        y,
        1,
        1,
        label,
        buttonStatus);
    screen->drawBitmap(
        x + (BUTTON_SIZE - ICON_SIZE) / 2,
        y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
        cog32x32,
        32,
        32,
        WHITE);
    previousValue = value;
}

void WidgetRecalibrateButton::refreshValue()
{
    value = true;
    if (!isTouched())
    {
        buttonStatus = ACTIVATED;
    }
}

boolean WidgetRecalibrateButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetRecalibrateButton::manageTouchDown()
{
}

void WidgetRecalibrateButton::manageTouchUp()
{
    TrackingObjectService::isTracking = false;
    ObjectToWatch *obj = new ObjectToWatch(trackedObject->type, trackedObject->code, trackedObject->label);
    CurrentViewService::changeCurrentView(new CalibrationView(screen, obj));
}
