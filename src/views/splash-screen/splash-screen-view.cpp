#include "splash-screen-view.h"
#include "colors.h"
#include "wifi-connection.h"
#include "current-view-service.h"
#include "calibration-view.h"
#include "gps.h"
#include "screen.h"
#include "image-check.h"
#include "secrets.h"

SplashScreenView::SplashScreenView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void SplashScreenView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
    screen->setTextFont(2);
    refreshStatuses(80, SCREEN_HEIGHT / 2);
}

void SplashScreenView::loop()
{
    boolean wifiStatus = WiFiConnection::isConnected();
    boolean gpsStatus = GPS::currentData.isValid;
    if (wifiStatus != currentWifiStatus || gpsStatus != currentGPSStatus)
    {
        refreshStatuses(80, SCREEN_HEIGHT / 2);
    }
    if (wifiStatus && gpsStatus)
    {
        CurrentViewService::changeCurrentView(
            new CalibrationView(
                this->screen));
    }
    currentWifiStatus = wifiStatus;
    currentGPSStatus = gpsStatus;
}

void SplashScreenView::refreshStatuses(int xPosition, int yPosition)
{
    int lineHeight = 24;
    screen->fillRect(
        0,
        yPosition,
        SCREEN_WIDTH,
        SCREEN_HEIGHT - yPosition,
        BACKGROUND_COLOR);

    screen->setCursor(xPosition, yPosition);
    screen->print("Initializing...");

    screen->setCursor(xPosition, yPosition + lineHeight);
    String wifiMessage;
    wifiMessage += F("Connection to WIFI network \"");
    wifiMessage += WIFI_SSID;
    wifiMessage += F("\"");
    screen->print(wifiMessage);
    if (WiFiConnection::isConnected())
    {
        screen->drawBitmap(
            xPosition - 50,
            yPosition + lineHeight,
            check32x32,
            32,
            32,
            GREEN);
    }

    screen->setCursor(xPosition, yPosition + (lineHeight * 2));
    String gpsMessage;
    gpsMessage += F("GPS acquiring satellites");
    screen->print(gpsMessage);
    if (GPS::currentData.isValid)
    {
        screen->drawBitmap(
            xPosition - 50,
            yPosition + (lineHeight * 2),
            check32x32,
            32,
            32,
            GREEN);
    }
}