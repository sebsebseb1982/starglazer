#include "tracking-object-service.h"
#include "duration.h"

TrackedObject *TrackingObjectService::trackedObject = nullptr;

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

void TrackingObjectService::setup(TrackedObject *trackedObject)
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

            startMillis = currentMillis;
        }
    }
    else
    {
        TrackingObjectService::isTracking = false;
    }
}