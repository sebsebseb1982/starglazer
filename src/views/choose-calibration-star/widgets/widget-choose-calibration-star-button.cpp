#include "widget-choose-calibration-star-button.h"
#include "debug.h"
#include "gui.h"
#include "colors.h"
#include "image-target.h"
#include "calibration-data.h"
#include "current-view-service.h"
// Forward declare to break circular include; full include in .cpp of the view
#include "choose-calibration-star-view.h"

WidgetChooseCalibrationStarButton::WidgetChooseCalibrationStarButton(
    unsigned int column,
    unsigned int row,
    ObjectToWatch star,
    int starIndex,
    bool unavailable,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs) : Widget(column,
                                              row,
                                              1,
                                              1,
                                              star.label,
                                              screen,
                                              refreshPeriodInMs),
                                       star(star),
                                       starIndex(starIndex),
                                       unavailable(unavailable),
                                       value(false),
                                       previousValue(false),
                                       buttonStatus(DESACTIVATED),
                                       previousButtonStatus(DESACTIVATED)
{
}

void WidgetChooseCalibrationStarButton::draw()
{
    ButtonStatus effectiveStatus = unavailable ? UNAVAILABLE : buttonStatus;
    gui.drawButton(
        x,
        y,
        1,
        1,
        label,
        effectiveStatus);
    screen->drawBitmap(
        x + (BUTTON_SIZE - ICON_SIZE) / 2,
        y + ((BUTTON_SIZE - ICON_SIZE) / 2) - 8,
        target32x32,
        32,
        32,
        unavailable ? DARK_GRAY : WHITE);
    previousValue = value;
}

void WidgetChooseCalibrationStarButton::refreshValue()
{
    value = true;
    if (!isTouched())
    {
        buttonStatus = ACTIVATED;
    }
}

boolean WidgetChooseCalibrationStarButton::isValueChanged()
{
    return value != previousValue;
}

void WidgetChooseCalibrationStarButton::manageTouchDown()
{
    if (unavailable)
        return;
    buttonStatus = ACTIVATED;
    draw();
}

void WidgetChooseCalibrationStarButton::manageTouchUp()
{
    if (unavailable)
        return;

    String msg;
    msg += F("CalibrationStar chosen: ");
    msg += star.label;
    DEBUG_PRINTLN(msg);

    ChooseCalibrationStarView::selectedStar = star;
}
