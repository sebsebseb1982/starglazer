#include "tracking-object-service.h"

TrackingObjectService::TrackingObjectService(TrackedObject *trackedObject) : trackedObject(trackedObject), equatorialCoordinatesService()
{
    isTracking = false;
}

void TrackingObjectService::setup(TrackedObject *trackedObject)
{
    this->trackedObject = trackedObject;
    isTracking = true;
}

void TrackingObjectService::loop()
{
    currentEquatorialCoordinates = equatorialCoordinatesService.compute(
        GPS::currentData,
        this->trackedObject);
}