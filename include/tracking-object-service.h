#ifndef TRACKING_OBJECT_SERVICE_H
#define TRACKING_OBJECT_SERVICE_H

#include "tracked-object.h"
#include "tracking-object-service.h"
#include "equatorial-coordinates-service.h"

class TrackingObjectService
{
private:
    TrackedObject *trackedObject;
    EquatorialCoordinatesService equatorialCoordinatesService;

public:
    EquatorialCoordinates currentEquatorialCoordinates;
    boolean isTracking;
    TrackingObjectService(TrackedObject *trackedObject);
    void setup(TrackedObject *trackedObject);
    void loop();
};

#endif
