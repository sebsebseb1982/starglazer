#include "current-view-service.h"
#include "tracking-object-view.h"

CurrentViewService::CurrentViewService(TFT_eSPI *screen)
{
    this->screen = screen;
}

void CurrentViewService::setup()
{
    Serial.println("CurrentViewService::setup()");
    TrackedObject trackedObject = TrackedObject(F("planets-and-moons"), F("sun"));
    this->currentView = new TrackingObjectView(this->screen, &trackedObject);
    this->currentView->setup();
}

void CurrentViewService::loop()
{
    this->currentView->loop();
}