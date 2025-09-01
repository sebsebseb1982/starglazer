#include "current-view-service.h"
#include "splash-screen-view.h"

View *CurrentViewService::currentView;

CurrentViewService::CurrentViewService(TFT_eSPI *screen)
{
    this->screen = screen;
}

void CurrentViewService::setup()
{
    changeCurrentView(
        new SplashScreenView(
            this->screen));
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