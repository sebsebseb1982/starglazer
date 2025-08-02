#include "widget-back-button.h"
#include "gui.h"
#include "colors.h"
#include "image-back.h"
#include "current-view-service.h"
#include "tracking-object-view.h"
#include "object-to-watch.h"

WidgetBackButton::WidgetBackButton(
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

void WidgetBackButton::draw()
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
        back32x32,
        32,
        32,
        WHITE);
    previousValue = value;
}

void WidgetBackButton::refreshValue()
{
    value = true;
    if (!isTouched())
    {
        buttonStatus = value ? ACTIVATED : DESACTIVATED;
    }
}

boolean WidgetBackButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetBackButton::manageTouchDown()
{

}

void WidgetBackButton::manageTouchUp()
{
    CurrentViewService ::changeCurrentView(
        new TrackingObjectView(
            this->screen,
            new ObjectToWatch(F("planets-and-moons"), F("moon"), F("Moon"))));
}
