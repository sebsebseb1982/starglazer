#include "widget-tracked-object-button.h"
#include "gui.h"
#include "colors.h"
#include "tracking-object-service.h"
#include "image-saturn.h"
#include "image-nebula.h"
#include "image-satellite.h"
#include "current-view-service.h"
#include "choosing-object-view.h"

WidgetTrackedObjectButton::WidgetTrackedObjectButton(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    ObjectToWatch *trackedObject,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              4,
                                              1,
                                              label,
                                              screen,
                                              refreshPeriodInMs)
{
    this->trackedObject = trackedObject;
}

void WidgetTrackedObjectButton::draw()
{
    gui.drawButton(
        x,
        y,
        columnSpan,
        rowSpan,
        label,
        UNAVAILABLE);

    uint32_t iconColor = VERY_LIGHT_GRAY;
    int iconSize = 32;
    if (trackedObject->type == "specials")
        screen->drawBitmap(
            x + 20,
            y + 22,
            satellite32x32,
            iconSize,
            iconSize,
            iconColor);
    else if (trackedObject->type == "deep-space-objects")
        screen->drawBitmap(
            x + 20,
            y + 22,
            nebula32x32,
            iconSize,
            iconSize,
            iconColor);
    else if (trackedObject->type == "planets-and-moons")
        screen->drawBitmap(
            x + 20,
            y + 22,
            saturn32x32,
            iconSize,
            iconSize,
            iconColor);

    String objectLabel = trackedObject->label;
    screen->setCursor(x + iconSize + 30, y + 20);
    screen->print(objectLabel);

    String objectCode = trackedObject->type;
    objectCode += F("/");
    objectCode += trackedObject->code;
    screen->setCursor(x + iconSize + 30, y + 38);
    screen->print(objectCode);

    previousValue = value;
}

void WidgetTrackedObjectButton::refreshValue()
{
}

boolean WidgetTrackedObjectButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetTrackedObjectButton::manageTouchDown()
{
}

void WidgetTrackedObjectButton::manageTouchUp()
{
    CurrentViewService::changeCurrentView(new ChoosingObjectView(screen));
}
