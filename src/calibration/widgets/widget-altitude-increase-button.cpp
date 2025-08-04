#include "widget-altitude-increase-button.h"
#include "gui.h"
#include "colors.h"
#include "image-arrow.h"
#include "gimbal.h"

WidgetAltitudeIncreaseButton::WidgetAltitudeIncreaseButton(
    unsigned int column,
    unsigned int row,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              "Altitude +",
                                              screen,
                                              refreshPeriodInMs)
{
    buttonStatus = DESACTIVATED;
}

void WidgetAltitudeIncreaseButton::draw()
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
        arrowUp32x32,
        32,
        32,
        WHITE);
}

void WidgetAltitudeIncreaseButton::refreshValue()
{
    if (buttonStatus == ACTIVATED)
    {
        Gimbal::altitudeMotor.rotateNSteps(10);
    }
}

boolean WidgetAltitudeIncreaseButton::isValueChanged()
{
    return false;
}

void WidgetAltitudeIncreaseButton::manageTouchDown()
{
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetAltitudeIncreaseButton::manageTouchUp()
{
    buttonStatus = DESACTIVATED;
    draw();
}
