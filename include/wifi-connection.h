#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <Arduino.h>

enum WiFiState
{
    WIFI_STATE_CONNECTED,
    WIFI_STATE_CONNECTING,
    WIFI_STATE_FAILED
};

class WiFiConnection {
  public:
    static unsigned int nbConnection;
    static WiFiState state;
    static void setup();
    static void loop();
    static boolean isConnected();
};

#endif