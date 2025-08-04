#include "calibration-view.h"
#include "colors.h"
#include "duration.h"
#include "widget-ok-button.h"
#include "widget-altitude-decrease-button.h"
#include "widget-altitude-increase-button.h"
#include "widget-azimuth-decrease-button.h"
#include "widget-azimuth-increase-button.h"
#include "current-view-service.h"
#include "choosing-object-view.h"
#include "gimbal.h"

boolean CalibrationView::calibrationDone = false;

CalibrationView::CalibrationView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void CalibrationView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);

    widgets.push_back(
        new WidgetOKButton(
            1,
            1,
            screen,
            THREE_HOURS));

    widgets.push_back(
        new WidgetAltitudeIncreaseButton(
            1,
            0,
            screen,
            HUNDRED_MILLISECONDS));

    widgets.push_back(
        new WidgetAltitudeDecreaseButton(
            1,
            2,
            screen,
            HUNDRED_MILLISECONDS));

    widgets.push_back(
        new WidgetAzimuthIncreaseButton(
            2,
            1,
            screen,
            HUNDRED_MILLISECONDS));

    widgets.push_back(
        new WidgetAzimuthDecreaseButton(
            0,
            1,
            screen,
            HUNDRED_MILLISECONDS));

    for (Widget *widget : widgets)
    {
        widget->init();
    }
}

void CalibrationView::loop()
{
    if (CalibrationView::calibrationDone)
    {
        CurrentViewService::changeCurrentView(new ChoosingObjectView(screen));
    }
    else
    {
        for (Widget *widget : widgets)
        {
            widget->refresh();
        }
        Gimbal::altitudeMotor.loop();
        Gimbal::azimuthMotor.loop();
    }
}

CalibrationView::~CalibrationView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
}