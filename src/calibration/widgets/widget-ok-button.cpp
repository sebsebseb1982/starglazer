#include "widget-ok-button.h"
#include "gui.h"
#include "colors.h"
#include "image-check.h"
#include "calibration-view.h"

WidgetOKButton::WidgetOKButton(
    unsigned int column,
    unsigned int row,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              "OK",
                                              screen,
                                              refreshPeriodInMs)
{
    buttonStatus = DESACTIVATED;
}

void WidgetOKButton::draw()
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
        check32x32,
        32,
        32,
        WHITE);
}

void WidgetOKButton::refreshValue()
{
}

boolean WidgetOKButton::isValueChanged()
{
    return false;
}

void WidgetOKButton::manageTouchDown()
{
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetOKButton::manageTouchUp()
{
    buttonStatus = DESACTIVATED;
    draw();
    CalibrationView::calibrationDone = true;
}
