#include "tracking-object-service.h"

TrackedObject* TrackingObjectService::trackedObject = nullptr;

EquatorialCoordinates TrackingObjectService::currentEquatorialCoordinates;
boolean TrackingObjectService::isTracking = false;

EquatorialCoordinatesService &TrackingObjectService::equatorialCoordinatesService = getEquatorialCoordinatesServiceInstance();

EquatorialCoordinatesService &TrackingObjectService::getEquatorialCoordinatesServiceInstance()
{
    static EquatorialCoordinatesService instance;
    return instance;
}

void TrackingObjectService::setup(TrackedObject *trackedObject)
{
    TrackingObjectService::trackedObject = trackedObject;
    TrackingObjectService::isTracking = true;
}

void TrackingObjectService::loop()
{
    TrackingObjectService::currentEquatorialCoordinates = TrackingObjectService::equatorialCoordinatesService.compute(
        GPS::currentData,
        TrackingObjectService::trackedObject);
}