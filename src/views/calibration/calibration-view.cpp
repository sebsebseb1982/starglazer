#include "calibration-view.h"
#include "colors.h"
#include "duration.h"
#include "widget-ok-button.h"
#include "widget-altitude-decrease-button.h"
#include "widget-altitude-increase-button.h"
#include "widget-azimuth-decrease-button.h"
#include "widget-azimuth-increase-button.h"
#include "widget-gps-status.h"
#include "current-view-service.h"
#include "choosing-object-view.h"
#include "gimbal.h"
#include "tracking-object-service.h"
#include "object-to-watch.h"
#include "laser.h"

boolean CalibrationView::calibrationDone = false;

CalibrationView::CalibrationView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void CalibrationView::setup()
{
    Laser::on();
    screen->fillScreen(BACKGROUND_COLOR);

    widgets.push_back(
        new WidgetOKButton(
            1,
            1,
            screen,
            HUNDRED_MILLISECONDS));

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

    widgets.push_back(
        new WidgetGPSStatus(
            4,
            0,
            "GPS",
            this->screen,
            FIVE_SECONDS));

    for (Widget *widget : widgets)
    {
        widget->init();
    }
}

void CalibrationView::loop()
{
    if (CalibrationView::calibrationDone)
    {
        Laser::off();
        static EquatorialCoordinatesService equatorialCoordinatesService;
        EquatorialCoordinates polarisEquatorialCoordinates = equatorialCoordinatesService.compute(
            GPS::currentData,
            new ObjectToWatch("deep-space-objects", "* alf UMi", "Polaris"));
        Gimbal::altitudeMotor.goToHome(polarisEquatorialCoordinates.altitude * -1.0);
        Gimbal::azimuthMotor.goToHome(polarisEquatorialCoordinates.azimuth);
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