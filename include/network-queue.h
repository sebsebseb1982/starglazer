#ifndef NETWORK_QUEUE_H
#define NETWORK_QUEUE_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "gps.h"
#include "equatorial-coordinates-service.h"
#include "object-to-watch.h"
#include "catalog.h"

#define NET_OBJ_MAX_LEN 128

enum NetworkRequestType
{
    NET_REQ_COMPUTE_COORDINATES,
    NET_REQ_API_HEALTH,
    NET_REQ_GET_CATALOG
};

struct NetworkCoordinatesPayload
{
    float latitude;
    float longitude;
    float elevation;
    bool gpsValid;
    char objType[NET_OBJ_MAX_LEN];
    char objCode[NET_OBJ_MAX_LEN];
    char objLabel[NET_OBJ_MAX_LEN];
};

struct NetworkRequest
{
    NetworkRequestType type;
    NetworkCoordinatesPayload coordinatesPayload;
};

class NetworkQueue
{
public:
    static QueueHandle_t requestQueue;

    // Results protected by resultsMutex — written by network task, read by UI task
    static SemaphoreHandle_t resultsMutex;

    static EquatorialCoordinates latestCoordinates;
    static volatile bool coordinatesReady;

    static volatile bool apiHealthy;
    static volatile bool healthReady;

    static Catalog latestCatalog;
    static volatile bool catalogReady;

    // Call once from setup(), creates network task on Core 1
    static void setup();

    // Network task entry point (do not call directly)
    static void networkTaskFunction(void *pvParameters);

    // Non-blocking send helpers — call from UI task (Core 0)
    static void sendComputeRequest(GPSData gpsData, ObjectToWatch *trackedObject);
    static void sendHealthRequest();
    static void sendCatalogRequest();

    // Non-blocking receive helpers — call from UI task (Core 0)
    // Returns true and populates 'out' if a fresh result is available, false otherwise
    static bool tryGetCoordinates(EquatorialCoordinates &out);
    static bool tryGetHealth(bool &out);
    static bool tryGetCatalog(Catalog &out);
};

#endif
