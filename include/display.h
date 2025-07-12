#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "gps.h"
#include "equatorial-coordinates-service.h"

class Display {
  private:
    TFT_eSPI *screen;
    EquatorialCoordinatesService equatorialCoordinatesService;
  public:
    Display(TFT_eSPI *screen);
    void setup();
    void loop(GPSData position);
};

#endif