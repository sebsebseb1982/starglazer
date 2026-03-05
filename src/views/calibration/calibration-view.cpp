#include "calibration-view.h"
#include "network-queue.h"
#include "debug.h"
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
#include "object-to-watch.h"
#include "laser.h"
#include "joystick.h"
#include "rgb-led.h"
#include "gps.h"

boolean CalibrationView::calibrationDone = false;

CalibrationView::CalibrationView(
    TFT_eSPI *screen) : state(CALIBRATION_STATE_CALIBRATING),
                        coordinatesRequestSent(false)
{
    this->screen = screen;
}

void CalibrationView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
    CalibrationView::calibrationDone = false;

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
    switch (state)
    {
    case CALIBRATION_STATE_CALIBRATING:
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
            Gimbal::altitudeMotor.rotateNSteps(-1.0 * maxSpeed * Joystick::status.up.value);
        if (Joystick::status.down.pressed)
            Gimbal::altitudeMotor.rotateNSteps(maxSpeed * Joystick::status.down.value);
        if (Joystick::status.left.pressed)
            Gimbal::azimuthMotor.rotateNSteps(maxSpeed * Joystick::status.left.value);
        if (Joystick::status.right.pressed)
            Gimbal::azimuthMotor.rotateNSteps(-1.0 * maxSpeed * Joystick::status.right.value);

        if (Joystick::status.cPressed)
            CalibrationView::calibrationDone = true;

        if (CalibrationView::calibrationDone)
        {
            Laser::off();
            RGBLed::off();
            coordinatesRequestSent = false;
            Gimbal::altitudeMotor.homingComplete = false;
            Gimbal::azimuthMotor.homingComplete = false;
            state = CALIBRATION_STATE_WAITING_COORDS;
        }
        else
        {
            for (Widget *widget : widgets)
            {
                DEBUG_PRINT("Refresh widget ");
                DEBUG_PRINTLN(widget->label);
                widget->refresh();
            }
        }
        break;
    }

    case CALIBRATION_STATE_WAITING_COORDS:
    {
        if (!coordinatesRequestSent)
        {
            ObjectToWatch polaris("deep-space-objects", "* alf UMi", "Polaris");
            NetworkQueue::sendComputeRequest(GPS::getDataSafe(), &polaris);
            coordinatesRequestSent = true;
            DEBUG_PRINTLN("CalibrationView: waiting for Polaris coordinates");
        }

        EquatorialCoordinates coords;
        if (NetworkQueue::tryGetCoordinates(coords))
        {
            float altHome = coords.altitude * -1.0;
            float azHome = coords.azimuth > 180.0
                               ? (360.0 - coords.azimuth) * -1.0
                               : coords.azimuth * -1.0;
            Gimbal::altitudeMotor.goToHome(altHome);
            Gimbal::azimuthMotor.goToHome(azHome);
            state = CALIBRATION_STATE_HOMING;
            DEBUG_PRINTLN("CalibrationView: homing started");
        }
        break;
    }

    case CALIBRATION_STATE_HOMING:
        if (Gimbal::altitudeMotor.homingComplete && Gimbal::azimuthMotor.homingComplete)
        {
            state = CALIBRATION_STATE_DONE;
        }
        break;

    case CALIBRATION_STATE_DONE:
        CurrentViewService::changeCurrentView(new ChoosingObjectView(screen));
        break;
    }
}

CalibrationView::~CalibrationView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
}
