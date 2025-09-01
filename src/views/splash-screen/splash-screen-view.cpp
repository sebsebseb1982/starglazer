#include "splash-screen-view.h"
#include "colors.h"
#include "wifi-connection.h"
#include "current-view-service.h"
#include "calibration-view.h"
#include "gps.h"
#include "screen.h"
#include "secrets.h"

SplashScreenView::SplashScreenView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void SplashScreenView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
    screen->setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    screen->print("Initializing...");
    refreshStatuses();
}

void SplashScreenView::loop()
{
    boolean wifiStatus = WiFiConnection::isConnected();
    boolean gpsStatus = GPS::currentData.isValid;
    if (wifiStatus != currentWifiStatus || gpsStatus != currentGPSStatus)
    {
        refreshStatuses();
    }
    if (wifiStatus && gpsStatus)
    {
        CurrentViewService::changeCurrentView(
            new CalibrationView(
                this->screen));
    }
}

void SplashScreenView::refreshStatuses()
{
    int lineHeight = 14;
    int xPostion = 100;
    int yPosition = SCREEN_HEIGHT / 2 + lineHeight;
    screen->fillRect(
        0,
        yPosition,
        SCREEN_WIDTH,
        SCREEN_HEIGHT - yPosition,
        BACKGROUND_COLOR);

    screen->setCursor(xPostion, yPosition + lineHeight);
    String wifiMessage;
    wifiMessage += F("Connection to WIFI network \"");
    wifiMessage += WIFI_SSID;
    wifiMessage += F("\" ... ");
    wifiMessage += WiFiConnection::isConnected() ? "OK" : "";
    screen->print(wifiMessage);
    
    screen->setCursor(xPostion, yPosition + (lineHeight*2));
    String gpsMessage;
    gpsMessage += F("GPS acquiring satellites ... ");
    gpsMessage += GPS::currentData.isValid ? "OK" : "";
    screen->print(gpsMessage);
}