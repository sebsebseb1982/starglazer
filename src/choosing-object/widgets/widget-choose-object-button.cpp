#include "widget-choose-object-button.h"
#include "gui.h"
#include "colors.h"
#include "image-satellite.h"
#include "choosing-object-view.h"

WidgetChooseObjectButton::WidgetChooseObjectButton(
    unsigned int column,
    unsigned int row,
    ObjectToWatch objectToWatch,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              objectToWatch.label,
                                              screen,
                                              refreshPeriodInMs),
                                       objectToWatch(objectToWatch)
{
}

void WidgetChooseObjectButton::draw()
{
    String message;
    message += F("WidgetChooseObjectButton::draw() ");
    message += this->objectToWatch.label;
    Serial.println(message);
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
        satellite32x32,
        32,
        32,
        WHITE);
    previousValue = value;
}

void WidgetChooseObjectButton::refreshValue()
{
    value = true;
    if (!isTouched())
    {
        buttonStatus = value ? ACTIVATED : DESACTIVATED;
    }
}

boolean WidgetChooseObjectButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetChooseObjectButton::manageTouchDown()
{
}

void WidgetChooseObjectButton::manageTouchUp()
{
    String message;
    message += F("Object choosen ");
    message += this->objectToWatch.label;
    Serial.println(message);
    ChoosingObjectView::objectToWatch = this->objectToWatch;
}
