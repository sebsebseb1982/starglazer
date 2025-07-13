#include "tracking-object-view.h"
#include "duration.h"

#include "widget-gps-status.h"
#include "widget-tracking-button.h"
#include "widget-tracking-status.h"

TrackingObjectView::TrackingObjectView(
    TFT_eSPI *screen,
    TrackedObject trackedObject) : trackedObject(trackedObject), equatorialCoordinatesService()
{
    this->screen = screen;
}

void TrackingObjectView::setup()
{
    Serial.println("TrackingObjectView::setup()");
    this->widgets[0] = new WidgetGPSStatus(4, 0, "GPS", this->screen, FIVE_SECONDS);
    this->widgets[1] = new WidgetTrackingButton(4, 2, "Tracking", this->screen, FIVE_SECONDS);
    this->widgets[2] = new WidgetTrackingStatus(0, 1, "Tracked object status", this->screen, &this->trackedObject, &this->equatorialCoordinatesService, FIVE_SECONDS);

    for (int i = 0; i < widgetNumbers; i++)
    {
        this->widgets[i]->init();
    }
}

void TrackingObjectView::loop()
{
    Serial.println("TrackingObjectView::loop()");
    for (int i = 0; i < widgetNumbers; i++)
    {
        if (this->widgets[i])
        {
            this->widgets[i]->refresh();
        }
    }
}