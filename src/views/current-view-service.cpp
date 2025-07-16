#include "current-view-service.h"
#include "tracking-object-view.h"

CurrentViewService::CurrentViewService(TFT_eSPI *screen)
{
    this->screen = screen;
}

void CurrentViewService::setup()
{
    this->currentView = new TrackingObjectView(this->screen, new TrackedObject(F("planets-and-moons"), F("moon")));
    this->currentView->setup();
}

void CurrentViewService::loop()
{
    this->currentView->loop();
}