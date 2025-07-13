#include "tracking-object-view.h"
#include "widget-gps-status.h"
#include "widget-tracking-button.h"
#include "duration.h"

TrackingObjectView::TrackingObjectView(TFT_eSPI *screen, TrackedObject trackedObject) : trackedObject(trackedObject)
{
    this->screen = screen;
}

void TrackingObjectView::setup()
{
    Serial.println("TrackingObjectView::setup()");
    this->widgets[0] = new WidgetGPSStatus(4, 0, "GPS", this->screen, FIVE_SECONDS);
    this->widgets[1] = new WidgetTrackingButton(4, 2, "tracking", this->screen, FIVE_SECONDS);

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
        if (this->widgets[i]) {
            this->widgets[i]->refresh();
        }
    }
}