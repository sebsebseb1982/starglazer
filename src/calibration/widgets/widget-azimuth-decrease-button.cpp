#include "widget-azimuth-decrease-button.h"
#include "gui.h"
#include "colors.h"
#include "image-arrow.h"

WidgetAzimuthDecreaseButton::WidgetAzimuthDecreaseButton(
    unsigned int column,
    unsigned int row,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              "Azimuth -",
                                              screen,
                                              refreshPeriodInMs)
{
    buttonStatus = DESACTIVATED;
}

void WidgetAzimuthDecreaseButton::draw()
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
        arrowLeft32x32,
        32,
        32,
        WHITE);
}

void WidgetAzimuthDecreaseButton::refreshValue()
{

}

boolean WidgetAzimuthDecreaseButton::isValueChanged()
{
    return false;
}

void WidgetAzimuthDecreaseButton::manageTouchDown()
{
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetAzimuthDecreaseButton::manageTouchUp()
{
    buttonStatus = DESACTIVATED;
    draw();
}
