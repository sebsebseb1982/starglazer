#ifndef GPS_H
#define GPS_H
#include <TinyGPSPlus.h>

class GPSData {
  private:
    float latitudeInDegrees;
    float longitudeInDegrees;
    float elevation;
  public:
    GPSData();
    GPSData(
      float latitudeInDegrees,
      float longitudeInDegrees,
      float elevation
    );
    float getLatitudeInDegrees();
    float getLongitudeInDegrees();
    float getLatitudeInRadians();
    float getLongitudeInRadians();
    float getElevation();
    void setup();
    void loop();
};

class GPS {
  private:
    TinyGPSPlus gps;
  public:
    GPS();
    GPSData currentData;
    bool ready;
    void setup();
    void loop();
};

#endif