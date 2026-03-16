#ifndef WIDGET_CHOOSE_CALIBRATION_STAR_BUTTON_H
#define WIDGET_CHOOSE_CALIBRATION_STAR_BUTTON_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "widget.h"
#include "object-to-watch.h"

class WidgetChooseCalibrationStarButton : public Widget
{
private:
    boolean value;
    boolean previousValue;
    ButtonStatus buttonStatus;
    ButtonStatus previousButtonStatus;
    ObjectToWatch star;
    int starIndex;
    bool unavailable;

public:
    WidgetChooseCalibrationStarButton(
        unsigned int column,
        unsigned int row,
        ObjectToWatch star,
        int starIndex,
        bool unavailable,
        TFT_eSPI *screen,
        unsigned long refreshPeriodInMs);
    void draw();
    void refreshValue();
    boolean isValueChanged();
    void manageTouchUp();
    void manageTouchDown();
};

#endif
