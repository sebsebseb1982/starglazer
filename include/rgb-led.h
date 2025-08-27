#ifndef RGB_LED_H
#define RGB_LED_H

#include <Arduino.h>

#define LED_RED_PIN 32
#define LED_GREEN_PIN 0
#define LED_BLUE_PIN 15

class RGBLed
{
private:
public:
    static void setup();
    static void red();
    static void green();
    static void blue();
    static void off();
};

#endif
