#include "widget-calibration-toggle-button.h"
#include "gui.h"
#include "colors.h"
#include "image-compass.h"

WidgetCalibrationToggleButton::WidgetCalibrationToggleButton(
    unsigned int column,
    unsigned int row,
    String label,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(
                                           column,
                                           row,
                                           1,
                                           1,
                                           label,
                                           screen,
                                           refreshPeriodInMs)
{
}

void WidgetCalibrationToggleButton::draw()
{
    gui.drawToggleButton(
        x,
        y,
        label,
        buttonStatus);
    screen->drawBitmap(
        x + (BUTTON_SIZE - 48) / 2,
        y + ((BUTTON_SIZE - 48) / 2) - 8,
        compass48x48,
        48,
        48,
        WHITE);
    previousValue = value;
}

void WidgetCalibrationToggleButton::refreshValue()
{
    value = CalibrationData::enabled;
    if (!isTouched())
    {
        buttonStatus = value ? ACTIVATED : DESACTIVATED;
    }
}

boolean WidgetCalibrationToggleButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetCalibrationToggleButton::manageTouchDown()
{
}

void WidgetCalibrationToggleButton::manageTouchUp()
{
    value = !value;
    CalibrationData::enabled = value;
    buttonStatus = value ? ACTIVATED : DESACTIVATED;
    draw();
}
