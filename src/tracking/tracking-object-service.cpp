#include "tracking-object-service.h"
#include "duration.h"

ObjectToWatch *TrackingObjectService::trackedObject = nullptr;

EquatorialCoordinates TrackingObjectService::currentEquatorialCoordinates;
boolean TrackingObjectService::isTracking = false;
boolean TrackingObjectService::isLaserPointingWanted = true;
EquatorialCoordinatesService &TrackingObjectService::equatorialCoordinatesService = getEquatorialCoordinatesServiceInstance();

Motor TrackingObjectService::altitudeMotor = Motor(33,
                                                   25,
                                                   3200,
                                                   32,
                                                   "Altitude");
Motor TrackingObjectService::azimuthMotor = Motor(27,
                                                  14,
                                                  3200,
                                                  32,
                                                  "Azimuth");

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
            altitudeMotor.goToAbsoluteAngle(currentEquatorialCoordinates.altitude);
            azimuthMotor.goToAbsoluteAngle(currentEquatorialCoordinates.azimuth * -1.0);
            startMillis = currentMillis;
        }
    }
    else
    {
        TrackingObjectService::isTracking = false;
    }

    if (TrackingObjectService::isTracking)
    {
        altitudeMotor.loop();
        azimuthMotor.loop();
    }
}