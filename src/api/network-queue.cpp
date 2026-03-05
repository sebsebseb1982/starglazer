#include "network-queue.h"
#include "starglaze-api.h"
#include "equatorial-coordinates-service.h"
#include "debug.h"

// Static member definitions
QueueHandle_t NetworkQueue::requestQueue = nullptr;
SemaphoreHandle_t NetworkQueue::resultsMutex = nullptr;

EquatorialCoordinates NetworkQueue::latestCoordinates;
volatile bool NetworkQueue::coordinatesReady = false;

volatile bool NetworkQueue::apiHealthy = false;
volatile bool NetworkQueue::healthReady = false;

Catalog NetworkQueue::latestCatalog;
volatile bool NetworkQueue::catalogReady = false;

void NetworkQueue::setup()
{
    requestQueue = xQueueCreate(8, sizeof(NetworkRequest));
    resultsMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
        NetworkQueue::networkTaskFunction,
        "networkTask",
        12000,
        NULL,
        3,
        NULL,
        1); // Core 1

    DEBUG_PRINTLN("NetworkQueue::setup() done");
}

void NetworkQueue::networkTaskFunction(void *pvParameters)
{
    // Service instances local to this task — no sharing across threads
    EquatorialCoordinatesService coordService;
    StarGlazeAPI api;

    NetworkRequest req;

    while (true)
    {
        if (xQueueReceive(requestQueue, &req, portMAX_DELAY) == pdTRUE)
        {
            if (req.type == NET_REQ_COMPUTE_COORDINATES)
            {
                DEBUG_PRINTLN("NetworkQueue: computing coordinates");
                GPSData gpsData(
                    req.coordinatesPayload.latitude,
                    req.coordinatesPayload.longitude,
                    req.coordinatesPayload.elevation,
                    req.coordinatesPayload.gpsValid);
                ObjectToWatch obj(
                    String(req.coordinatesPayload.objType),
                    String(req.coordinatesPayload.objCode),
                    String(req.coordinatesPayload.objLabel));

                EquatorialCoordinates result = coordService.compute(gpsData, &obj);

                if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(1000)) == pdTRUE)
                {
                    latestCoordinates = result;
                    coordinatesReady = true;
                    xSemaphoreGive(resultsMutex);
                }
            }
            else if (req.type == NET_REQ_API_HEALTH)
            {
                DEBUG_PRINTLN("NetworkQueue: checking API health");
                bool healthy = api.health();

                if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(1000)) == pdTRUE)
                {
                    apiHealthy = healthy;
                    healthReady = true;
                    xSemaphoreGive(resultsMutex);
                }
            }
            else if (req.type == NET_REQ_GET_CATALOG)
            {
                DEBUG_PRINTLN("NetworkQueue: fetching catalog");
                Catalog result = api.getCatalog();

                if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(1000)) == pdTRUE)
                {
                    latestCatalog = result;
                    catalogReady = true;
                    xSemaphoreGive(resultsMutex);
                }
            }
        }
    }
}

void NetworkQueue::sendComputeRequest(GPSData gpsData, ObjectToWatch *trackedObject)
{
    if (requestQueue == nullptr)
        return;

    NetworkRequest req;
    req.type = NET_REQ_COMPUTE_COORDINATES;
    req.coordinatesPayload.latitude = gpsData.latitudeInDegrees;
    req.coordinatesPayload.longitude = gpsData.longitudeInDegrees;
    req.coordinatesPayload.elevation = gpsData.elevation;
    req.coordinatesPayload.gpsValid = gpsData.isValid;

    strncpy(req.coordinatesPayload.objType, trackedObject->type.c_str(), NET_OBJ_MAX_LEN - 1);
    strncpy(req.coordinatesPayload.objCode, trackedObject->code.c_str(), NET_OBJ_MAX_LEN - 1);
    strncpy(req.coordinatesPayload.objLabel, trackedObject->label.c_str(), NET_OBJ_MAX_LEN - 1);
    req.coordinatesPayload.objType[NET_OBJ_MAX_LEN - 1] = '\0';
    req.coordinatesPayload.objCode[NET_OBJ_MAX_LEN - 1] = '\0';
    req.coordinatesPayload.objLabel[NET_OBJ_MAX_LEN - 1] = '\0';

    xQueueSend(requestQueue, &req, 0); // non-blocking
}

void NetworkQueue::sendHealthRequest()
{
    if (requestQueue == nullptr)
        return;

    NetworkRequest req;
    req.type = NET_REQ_API_HEALTH;
    xQueueSend(requestQueue, &req, 0);
}

void NetworkQueue::sendCatalogRequest()
{
    if (requestQueue == nullptr)
        return;

    NetworkRequest req;
    req.type = NET_REQ_GET_CATALOG;
    xQueueSend(requestQueue, &req, 0);
}

bool NetworkQueue::tryGetCoordinates(EquatorialCoordinates &out)
{
    if (!coordinatesReady)
        return false;

    if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        if (coordinatesReady)
        {
            out = latestCoordinates;
            coordinatesReady = false;
            xSemaphoreGive(resultsMutex);
            return true;
        }
        xSemaphoreGive(resultsMutex);
    }
    return false;
}

bool NetworkQueue::tryGetHealth(bool &out)
{
    if (!healthReady)
        return false;

    if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        if (healthReady)
        {
            out = apiHealthy;
            healthReady = false;
            xSemaphoreGive(resultsMutex);
            return true;
        }
        xSemaphoreGive(resultsMutex);
    }
    return false;
}

bool NetworkQueue::tryGetCatalog(Catalog &out)
{
    if (!catalogReady)
        return false;

    if (xSemaphoreTake(resultsMutex, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        if (catalogReady)
        {
            out = latestCatalog;
            catalogReady = false;
            xSemaphoreGive(resultsMutex);
            return true;
        }
        xSemaphoreGive(resultsMutex);
    }
    return false;
}
