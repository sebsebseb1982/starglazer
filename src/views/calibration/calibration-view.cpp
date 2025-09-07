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
#include "joystick.h"
#include "rgb-led.h"

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
    if (Joystick::status.zPressed)
    {
        Laser::on();
        RGBLed::green();
    }
    else
    {
        Laser::off();
        RGBLed::off();
    }

    int maxSpeed = 5;

    if (Joystick::status.up.pressed)
    {
        Gimbal::altitudeMotor.rotateNSteps(-1.0 * maxSpeed * Joystick::status.up.value);
    }

    if (Joystick::status.down.pressed)
    {
        Gimbal::altitudeMotor.rotateNSteps(maxSpeed * Joystick::status.down.value);
    }

    if (Joystick::status.left.pressed)
    {
        Gimbal::azimuthMotor.rotateNSteps(maxSpeed * Joystick::status.left.value);
    }

    if (Joystick::status.right.pressed)
    {
        Gimbal::azimuthMotor.rotateNSteps(-1.0 * maxSpeed * Joystick::status.right.value);
    }

    if (CalibrationView::calibrationDone)
    {
        Laser::off();
        static EquatorialCoordinatesService equatorialCoordinatesService;
        EquatorialCoordinates polarisEquatorialCoordinates = equatorialCoordinatesService.compute(
            GPS::currentData,
            new ObjectToWatch("deep-space-objects", "* alf UMi", "Polaris"));
        Gimbal::altitudeMotor.goToHome(polarisEquatorialCoordinates.altitude * -1.0);
        if (polarisEquatorialCoordinates.azimuth > 180.0)
        {
            Gimbal::azimuthMotor.goToHome((360.0 - polarisEquatorialCoordinates.azimuth) * -1.0);
        }
        else
        {
            Gimbal::azimuthMotor.goToHome(polarisEquatorialCoordinates.azimuth * -1.0);
        }

        CurrentViewService::changeCurrentView(new ChoosingObjectView(screen));
    }
    else
    {
        for (Widget *widget : widgets)
        {
            Serial.print("Refresh widget ");
            Serial.println(widget->label);
            widget->refresh();
        }
        Gimbal::altitudeMotor.loop();
        Gimbal::azimuthMotor.loop();
    }

    if (Joystick::status.cPressed)
    {
        CalibrationView::calibrationDone = true;
    }
}

CalibrationView::~CalibrationView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
}