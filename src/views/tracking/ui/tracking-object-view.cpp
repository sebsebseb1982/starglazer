#include "tracking-object-view.h"
#include "duration.h"

#include "widget-gps-status.h"
#include "widget-tracking-button.h"
#include "widget-tracking-status.h"
#include "widget-wifi-status.h"
#include "widget-api-status.h"
#include "widget-laser-button.h"
#include "widget-tracked-object-button.h"

#include "joystick.h"

TrackingObjectView::TrackingObjectView(
    TFT_eSPI *screen,
    ObjectToWatch *trackedObject) : trackedObject(trackedObject)
{
    this->screen = screen;
}

void TrackingObjectView::setup()
{
    TrackingObjectService::setup(trackedObject);
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
        HUNDRED_MILLISECONDS);
    this->widgets[2] = new WidgetTrackingStatus(
        0,
        1,
        "Tracked object status",
        this->screen,
        this->trackedObject,
        THIRTHY_SECONDS);
    this->widgets[3] = new WidgetWifiStatus(
        5,
        2,
        "Wifi",
        this->screen,
        ONE_SECOND);
    this->widgets[4] = new WidgetAPIStatus(
        5,
        3,
        "API",
        this->screen,
        FIVE_SECONDS);
    this->widgets[5] = new WidgetLaserButton(
        4,
        3,
        "Laser",
        this->screen,
        HUNDRED_MILLISECONDS);
    this->widgets[6] = new WidgetTrackedObjectButton(
        0,
        0,
        "Tracked object",
        this->screen,
        this->trackedObject,
        THREE_HOURS);

    for (int i = 0; i < widgetNumbers; i++)
    {
        this->widgets[i]->init();
    }
}

void TrackingObjectView::loop()
{
    TrackingObjectService::isLaserPointingWanted = Joystick::status.zPressed;
    TrackingObjectService::loop();
    for (int i = 0; i < widgetNumbers; i++)
    {
        if (this->widgets[i])
        {
            this->widgets[i]->refresh();
        }
    }
}