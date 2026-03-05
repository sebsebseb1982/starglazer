#ifndef TRACKING_OBJECT_SERVICE_H
#define TRACKING_OBJECT_SERVICE_H

#include "object-to-watch.h"
#include "tracking-object-service.h"
#include "equatorial-coordinates-service.h"

class TrackingObjectService
{
private:
    static ObjectToWatch *trackedObject;
    static unsigned long startMillis;
    static bool pendingCoordinatesRequest;

public:
    static EquatorialCoordinates currentEquatorialCoordinates;
    static boolean isTracking;
    static boolean isLaserPointingWanted;
    static void setup(ObjectToWatch *trackedObject);
    static void loop();
};

#endif
