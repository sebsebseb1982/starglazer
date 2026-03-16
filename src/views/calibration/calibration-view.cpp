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
#include "choose-calibration-star-view.h"
#include "choosing-object-view.h"
#include "tracking-object-view.h"
#include "gimbal.h"
#include "calibration-data.h"
#include "laser.h"
#include "joystick.h"
#include "rgb-led.h"
#include "gps.h"
#include "screen.h"

boolean CalibrationView::calibrationDone = false;

CalibrationView::CalibrationView(
    TFT_eSPI *screen,
    int starIndex,
    ObjectToWatch *returnToObject) : starIndex(starIndex),
                                     returnToObject(returnToObject),
                                     coordinatesRequestSent(false),
                                     coordsReceived(false),
                                     starApiAlt(0),
                                     starApiAz(0),
                                     angleBeforeAlt(0),
                                     angleBeforeAz(0)
{
    this->screen = screen;

    // Star 0 without recalibration starts directly in CALIBRATING so the user
    // manually points before any home is set.
    // Star 0 with recalibration, and all subsequent stars, start by auto-moving.
    if (starIndex == 0 && returnToObject == nullptr)
        state = CALIBRATION_STATE_CALIBRATING;
    else
        state = CALIBRATION_STATE_RESTORING_POSITION;
}

void CalibrationView::drawHeader()
{
    const String &name = CalibrationData::points[starIndex].star.label;
    screen->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    screen->setTextFont(2);

    // Draw star name + progress in the lower right area (row 3, cols 3-5)
    screen->fillRect(3 * BUTTON_SIZE, 3 * BUTTON_SIZE, 3 * BUTTON_SIZE, BUTTON_SIZE, BACKGROUND_COLOR);
    screen->setCursor(3 * BUTTON_SIZE + 4, 3 * BUTTON_SIZE + 8);
    screen->print(name);
    screen->print(" (");
    screen->print(starIndex + 1);
    screen->print("/3)");
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

    drawHeader();
}

void CalibrationView::loop()
{
    switch (state)
    {
    case CALIBRATION_STATE_RESTORING_POSITION:
    {
        if (!coordinatesRequestSent)
        {
            NetworkQueue::sendComputeRequest(GPS::getDataSafe(), &CalibrationData::points[starIndex].star);
            coordinatesRequestSent = true;

            String msg;
            msg += F("CalibrationView: fetching coords for ");
            msg += CalibrationData::points[starIndex].star.label;
            DEBUG_PRINTLN(msg);
        }

        EquatorialCoordinates coords;
        if (!coordsReceived && NetworkQueue::tryGetCoordinates(coords))
        {
            starApiAlt = coords.altitude;
            starApiAz = coords.azimuth;
            Gimbal::altitudeMotor.goToAbsoluteAngle(coords.altitude);
            Gimbal::azimuthMotor.goToAbsoluteAngle(coords.azimuth * -1.0);
            Laser::on();
            RGBLed::green();
            coordsReceived = true;
            DEBUG_PRINTLN("CalibrationView: moving to star position");
        }

        if (coordsReceived &&
            !Gimbal::altitudeMotor.isMoving() &&
            !Gimbal::azimuthMotor.isMoving())
        {
            // Record motor angles before user fine-tunes (used for correction on stars 1 and 2)
            if (starIndex > 0)
            {
                angleBeforeAlt = Gimbal::altitudeMotor.getCurrentAngle();
                angleBeforeAz  = Gimbal::azimuthMotor.getCurrentAngle();
            }
            coordinatesRequestSent = false;
            coordsReceived = false;
            state = CALIBRATION_STATE_CALIBRATING;
            DEBUG_PRINTLN("CalibrationView: at star position, ready to calibrate");
        }
        break;
    }

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

            if (starIndex == 0)
            {
                // First star: fetch API coords to compute the home offset
                coordinatesRequestSent = false;
                Gimbal::altitudeMotor.homingComplete = false;
                Gimbal::azimuthMotor.homingComplete = false;
                state = CALIBRATION_STATE_WAITING_COORDS;
            }
            else
            {
                // Stars 1 and 2: measure correction as the angular delta the
                // user applied relative to the auto-moved reference position
                float angleAfterAlt = Gimbal::altitudeMotor.getCurrentAngle();
                float angleAfterAz  = Gimbal::azimuthMotor.getCurrentAngle();

                CalibrationPoint point = CalibrationData::points[starIndex];
                point.apiAlt = starApiAlt;
                point.apiAz  = starApiAz;
                point.correctionAlt = angleAfterAlt - angleBeforeAlt;
                point.correctionAz  = -(angleAfterAz - angleBeforeAz);
                CalibrationData::recordPoint(starIndex, point);

                if (starIndex == 2)
                    CalibrationData::computeCorrection();

                state = CALIBRATION_STATE_DONE;
            }
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
        // Only reached for starIndex == 0 to compute the home offset
        if (!coordinatesRequestSent)
        {
            NetworkQueue::sendComputeRequest(GPS::getDataSafe(), &CalibrationData::points[0].star);
            coordinatesRequestSent = true;
            DEBUG_PRINTLN("CalibrationView: waiting for home coords");
        }

        EquatorialCoordinates coords;
        if (NetworkQueue::tryGetCoordinates(coords))
        {
            // Record star 0 with zero correction (it is the reference)
            CalibrationPoint point = CalibrationData::points[0];
            point.apiAlt = coords.altitude;
            point.apiAz  = coords.azimuth;
            point.correctionAlt = 0;
            point.correctionAz  = 0;
            CalibrationData::recordPoint(0, point);

            float altHome = coords.altitude * -1.0;
            float azHome = coords.azimuth > 180.0
                               ? coords.azimuth - 360.0
                               : coords.azimuth;
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
    {
        if (starIndex < 2)
        {
            // Proceed to select the next calibration star
            ObjectToWatch *ret = returnToObject;
            returnToObject = nullptr;
            CurrentViewService::changeCurrentView(
                new ChooseCalibrationStarView(screen, starIndex + 1, ret));
        }
        else if (returnToObject != nullptr)
        {
            ObjectToWatch *obj = returnToObject;
            returnToObject = nullptr;
            CurrentViewService::changeCurrentView(new TrackingObjectView(screen, obj));
        }
        else
        {
            CurrentViewService::changeCurrentView(new ChoosingObjectView(screen));
        }
        break;
    }
    }
}

CalibrationView::~CalibrationView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
    if (returnToObject != nullptr)
    {
        delete returnToObject;
        returnToObject = nullptr;
    }
}
