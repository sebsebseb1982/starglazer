#include "rgb-led.h"
#include "pinout.h"
#include <Arduino.h>

void RGBLed::setup()
{
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);
    off();
}

void RGBLed::red()
{
    off();
    digitalWrite(LED_RED_PIN, HIGH);
}

void RGBLed::green()
{
    off();
    digitalWrite(LED_GREEN_PIN, HIGH);
}

void RGBLed::blue()
{
    off();
    digitalWrite(LED_BLUE_PIN, HIGH);
}

void RGBLed::off()
{
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}
