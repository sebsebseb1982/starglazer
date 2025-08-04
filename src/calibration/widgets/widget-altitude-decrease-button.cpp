#include "widget-altitude-decrease-button.h"
#include "gui.h"
#include "colors.h"
#include "image-arrow.h"

WidgetAltitudeDecreaseButton::WidgetAltitudeDecreaseButton(
    unsigned int column,
    unsigned int row,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              "Altitude -",
                                              screen,
                                              refreshPeriodInMs)
{
    buttonStatus = DESACTIVATED;
}

void WidgetAltitudeDecreaseButton::draw()
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
        arrowBottom32x32,
        32,
        32,
        WHITE);
}

void WidgetAltitudeDecreaseButton::refreshValue()
{

}

boolean WidgetAltitudeDecreaseButton::isValueChanged()
{
    return false;
}

void WidgetAltitudeDecreaseButton::manageTouchDown()
{
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetAltitudeDecreaseButton::manageTouchUp()
{
    buttonStatus = DESACTIVATED;
    draw();
}
