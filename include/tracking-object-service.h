#ifndef TRACKING_OBJECT_SERVICE_H
#define TRACKING_OBJECT_SERVICE_H

#include "tracked-object.h"
#include "tracking-object-service.h"
#include "equatorial-coordinates-service.h"

class TrackingObjectService
{
private:
    static TrackedObject *trackedObject;
    static EquatorialCoordinatesService &equatorialCoordinatesService;
    static EquatorialCoordinatesService &getEquatorialCoordinatesServiceInstance();

public:
    static EquatorialCoordinates currentEquatorialCoordinates;
    static boolean isTracking;
    static void setup(TrackedObject *trackedObject);
    static void loop();
};

#endif
