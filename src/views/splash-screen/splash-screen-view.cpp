#include "splash-screen-view.h"
#include "colors.h"
#include "wifi-connection.h"
#include "current-view-service.h"
#include "calibration-view.h"
#include "gps.h"

SplashScreenView::SplashScreenView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void SplashScreenView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
    screen->setCursor(10, 10);
    screen->print("Initializing...");
}

void SplashScreenView::loop()
{
    if(WiFiConnection::isConnected() && GPS::currentData.isValid) {
          CurrentViewService::changeCurrentView(
            new CalibrationView(
            this->screen));
    }
}
