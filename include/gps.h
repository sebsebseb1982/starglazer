#ifndef GPS_H
#define GPS_H
#include <TinyGPSPlus.h>

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
    static GPSData currentData;
    static void setup();
    static void loop();
};

#endif