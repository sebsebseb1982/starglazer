#ifndef TOUCHSCREEN2_H
#define TOUCHSCREEN2_H

#include <Arduino.h>
#include <TFT_eSPI.h>

#define PEN_PIN 27

class TouchScreen
{
private:
  static TFT_eSPI *screen;
  static void IRAM_ATTR onTouchEvent();
  static unsigned long lastHighTime;
  static unsigned long lastLowTime;
  static unsigned long debounceDelay;

public:
  static uint16_t x;
  static uint16_t y;
  static boolean isTouched;
  static void setup(TFT_eSPI *screen);
  static void loop();
};

#endif
