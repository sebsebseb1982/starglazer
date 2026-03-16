#ifndef WIDGET_CALIBRATION_TOGGLE_BUTTON_H
#define WIDGET_CALIBRATION_TOGGLE_BUTTON_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "calibration-data.h"

class WidgetCalibrationToggleButton : public Widget
{
private:
    boolean value;
    boolean previousValue;
    ButtonStatus buttonStatus;
    ButtonStatus previousButtonStatus;

public:
    WidgetCalibrationToggleButton(
        unsigned int column,
        unsigned int row,
        String label,
        TFT_eSPI *screen,
        unsigned long refreshPeriodInMs);
    void draw();
    void refreshValue();
    boolean isValueChanged();
    void manageTouchUp();
    void manageTouchDown();
};

#endif
