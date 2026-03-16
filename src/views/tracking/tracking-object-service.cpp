#include "tracking-object-service.h"
#include "network-queue.h"
#include "duration.h"
#include "gimbal.h"
#include "laser.h"
#include "gps.h"
#include "calibration-data.h"

ObjectToWatch *TrackingObjectService::trackedObject = nullptr;
EquatorialCoordinates TrackingObjectService::currentEquatorialCoordinates;
boolean TrackingObjectService::isTracking = false;
boolean TrackingObjectService::isLaserPointingWanted = true;
unsigned long TrackingObjectService::startMillis = 0;
bool TrackingObjectService::pendingCoordinatesRequest = false;

void TrackingObjectService::setup(ObjectToWatch *trackedObject)
{
    TrackingObjectService::isTracking = GPS::getDataSafe().isValid;
    TrackingObjectService::trackedObject = trackedObject;
    TrackingObjectService::isLaserPointingWanted = false;
    TrackingObjectService::pendingCoordinatesRequest = false;
    TrackingObjectService::startMillis = 0;
}

void TrackingObjectService::loop()
{
    // Collect coordinate result if available
    EquatorialCoordinates newCoords;
    if (NetworkQueue::tryGetCoordinates(newCoords))
    {
        currentEquatorialCoordinates = newCoords;
        float corrAlt, corrAz;
        CalibrationData::applyCorrection(
            currentEquatorialCoordinates.altitude,
            currentEquatorialCoordinates.azimuth,
            corrAlt,
            corrAz);
        Gimbal::altitudeMotor.goToAbsoluteAngle(corrAlt);
        Gimbal::azimuthMotor.goToAbsoluteAngle(corrAz * -1.0);
        pendingCoordinatesRequest = false;
    }

    GPSData gpsData = GPS::getDataSafe();
    if (gpsData.isValid)
    {
        isTracking = true;
        unsigned long currentMillis = millis();
        if (!pendingCoordinatesRequest && (currentMillis - startMillis >= FIVE_SECONDS))
        {
            NetworkQueue::sendComputeRequest(gpsData, TrackingObjectService::trackedObject);
            pendingCoordinatesRequest = true;
            startMillis = currentMillis;
        }
    }
    else
    {
        isTracking = false;
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