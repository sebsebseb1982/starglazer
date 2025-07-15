#include "tracking-object-view.h"
#include "duration.h"

#include "widget-gps-status.h"
#include "widget-tracking-button.h"
#include "widget-tracking-status.h"
#include "widget-wifi-status.h"

TrackingObjectView::TrackingObjectView(
    TFT_eSPI *screen,
    TrackedObject *trackedObject) : trackedObject(trackedObject)
{
    this->screen = screen;
    this->trackingObjectService = new TrackingObjectService(trackedObject);
}

void TrackingObjectView::setup()
{
    trackingObjectService->setup(trackedObject);
    this->widgets[0] = new WidgetGPSStatus(
        4,
        0,
        "GPS",
        this->screen,
        FIVE_SECONDS);
    this->widgets[1] = new WidgetTrackingButton(
        4,
        2,
        "Tracking",
        this->screen,
        this->trackingObjectService,
        FIVE_SECONDS);
    this->widgets[2] = new WidgetTrackingStatus(
        0,
        1,
        "Tracked object status",
        this->screen,
        this->trackedObject,
        this->trackingObjectService,
        FIVE_SECONDS);
    this->widgets[3] = new WidgetWifiStatus(
        5,
        2,
        "Wifi",
        this->screen,
        ONE_SECOND);

    for (int i = 0; i < widgetNumbers; i++)
    {
        this->widgets[i]->init();
    }
}

void TrackingObjectView::loop()
{
    trackingObjectService->loop();
    for (int i = 0; i < widgetNumbers; i++)
    {
        if (this->widgets[i])
        {
            this->widgets[i]->refresh();
        }
    }
}