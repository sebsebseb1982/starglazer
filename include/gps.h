#ifndef GPS_H
#define GPS_H
#include <TinyGPSPlus.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class GPSData {
  private:

  public:
    float latitudeInDegrees;
    float longitudeInDegrees;
    float elevation;
    boolean isValid;
    GPSData();
    GPSData(
      float latitudeInDegrees,
      float longitudeInDegrees,
      float elevation,
      boolean isValid
    );
};

class GPS {
  private:
    static unsigned long startMillis;

  public:
    static TinyGPSPlus gps;
    static GPSData currentData;     // write from Core 1 only
    static SemaphoreHandle_t dataMutex;
    static void setup();
    static void loop();
    // Thread-safe read — always use this from Core 0
    static GPSData getDataSafe();
};

#endif