#include "widget-azimuth-increase-button.h"
#include "gui.h"
#include "colors.h"
#include "image-arrow.h"
#include "gimbal.h"

WidgetAzimuthIncreaseButton::WidgetAzimuthIncreaseButton(
    unsigned int column,
    unsigned int row,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              "Azimuth +",
                                              screen,
                                              refreshPeriodInMs)
{
    buttonStatus = DESACTIVATED;
}

void WidgetAzimuthIncreaseButton::draw()
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
        arrowRight32x32,
        32,
        32,
        WHITE);
}

void WidgetAzimuthIncreaseButton::refreshValue()
{
    if (buttonStatus == ACTIVATED)
    {
        Gimbal::azimuthMotor.rotateNSteps(10);
    }
}

boolean WidgetAzimuthIncreaseButton::isValueChanged()
{
    return false;
}

void WidgetAzimuthIncreaseButton::manageTouchDown()
{
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetAzimuthIncreaseButton::manageTouchUp()
{
    buttonStatus = DESACTIVATED;
    draw();
}
