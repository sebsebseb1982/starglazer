#include "current-view-service.h"
#include "tracking-object-view.h"

View *CurrentViewService::currentView;

CurrentViewService::CurrentViewService(TFT_eSPI *screen)
{
    this->screen = screen;
}

void CurrentViewService::setup()
{
    changeCurrentView(
        new TrackingObjectView(
            this->screen,
            new ObjectToWatch(F("planets-and-moons"), F("sun"), F("Sun"))));
}

void CurrentViewService::loop()
{
    CurrentViewService::currentView->loop();
}

void CurrentViewService::changeCurrentView(View *newView)
{
    delete CurrentViewService::currentView;
    CurrentViewService::currentView = newView;
    CurrentViewService::currentView->setup();
}