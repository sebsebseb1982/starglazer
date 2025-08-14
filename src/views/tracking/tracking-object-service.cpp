#include "tracking-object-service.h"
#include "duration.h"
#include "gimbal.h"
#include "laser.h"

ObjectToWatch *TrackingObjectService::trackedObject = nullptr;

EquatorialCoordinates TrackingObjectService::currentEquatorialCoordinates;
boolean TrackingObjectService::isTracking = false;
boolean TrackingObjectService::isLaserPointingWanted = true;
EquatorialCoordinatesService &TrackingObjectService::equatorialCoordinatesService = getEquatorialCoordinatesServiceInstance();

EquatorialCoordinatesService &TrackingObjectService::getEquatorialCoordinatesServiceInstance()
{
    static EquatorialCoordinatesService instance;
    return instance;
}

unsigned long TrackingObjectService::startMillis = 0;

void TrackingObjectService::setup(ObjectToWatch *trackedObject)
{
    TrackingObjectService::isTracking = GPS::currentData.isValid;
    TrackingObjectService::trackedObject = trackedObject;
    TrackingObjectService::isLaserPointingWanted = false;
}

void TrackingObjectService::loop()
{
    if (GPS::currentData.isValid)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - startMillis >= FIVE_SECONDS)
        {
            TrackingObjectService::currentEquatorialCoordinates = TrackingObjectService::equatorialCoordinatesService.compute(
                GPS::currentData,
                TrackingObjectService::trackedObject);
            Gimbal::altitudeMotor.goToAbsoluteAngle(currentEquatorialCoordinates.altitude);
            Gimbal::azimuthMotor.goToAbsoluteAngle(currentEquatorialCoordinates.azimuth * -1.0);
            startMillis = currentMillis;
        }
    }
    else
    {
        TrackingObjectService::isTracking = false;
    }

    if (TrackingObjectService::isTracking)
    {
        Gimbal::altitudeMotor.loop();
        Gimbal::azimuthMotor.loop();
    }

    if (TrackingObjectService::isLaserPointingWanted)
    {
        Laser::on();
    }
    else
    {
        Laser::off();
    }
}