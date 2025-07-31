#include <TFT_eSPI.h>
#include "touch-screen.h"
#include "screen.h"

TFT_eSPI *TouchScreen::screen = nullptr;
uint16_t TouchScreen::x = 0;
uint16_t TouchScreen::y = 0;
boolean TouchScreen::isTouched = false;
unsigned long TouchScreen::lastLowTime = 0;
unsigned long TouchScreen::lastHighTime = 0;
unsigned long TouchScreen::debounceDelay = 5;
volatile bool touchFlag = false;

void TouchScreen::setup(TFT_eSPI *screen)
{
  TouchScreen::screen = screen;
  pinMode(PEN_PIN, INPUT_PULLDOWN);
  attachInterrupt(PEN_PIN, TouchScreen::onTouchEvent, CHANGE);
}

void IRAM_ATTR TouchScreen::onTouchEvent()
{
  touchFlag = true; // Set flag for main loop
}


void TouchScreen::loop()
{
  static unsigned long lastChange = 0;
  static bool lastState = HIGH;
  if (!touchFlag)
    return;

  touchFlag = false;

  unsigned long now = millis();
  bool reading = digitalRead(PEN_PIN);
  if (reading != lastState && (now - lastChange) > TouchScreen::debounceDelay)
  {
    lastChange = now;
    lastState = reading;
    if (reading == LOW)
    {
      // Touch detected
      if (TouchScreen::screen->getTouch(&TouchScreen::x, &TouchScreen::y))
      {
        TouchScreen::x = SCREEN_WIDTH - TouchScreen::x;
        TouchScreen::y = SCREEN_HEIGHT - TouchScreen::y;
        TouchScreen::isTouched = true;
        Serial.println("Touch detected");
      }
    }
    else
    {
      // Released
      TouchScreen::isTouched = false;
      Serial.println("Touch released");
    }
  }
}