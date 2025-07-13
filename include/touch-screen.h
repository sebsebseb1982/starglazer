#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <TFT_eSPI.h>

class TouchScreen {
  public:
    static uint16_t x;
    static uint16_t y;
    static boolean isTouched;
    static void refresh(TFT_eSPI *screen);
};

#endif
