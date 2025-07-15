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

void TouchScreen::setup(TFT_eSPI *screen)
{
  TouchScreen::screen = screen;
  pinMode(PEN_PIN, INPUT_PULLDOWN);
  attachInterrupt(PEN_PIN, TouchScreen::onTouchEvent, CHANGE);
}

void IRAM_ATTR TouchScreen::onTouchEvent()
{

  int reading = digitalRead(PEN_PIN);

  unsigned long now = millis();

  if (reading == LOW)
  {
    Serial.println("LOW");
    if ((now - TouchScreen::lastHighTime) > TouchScreen::debounceDelay)
    {
      Serial.println("touched");
      TouchScreen::isTouched = TouchScreen::screen->getTouch(&TouchScreen::x, &TouchScreen::y);
      TouchScreen::x = SCREEN_WIDTH - TouchScreen::x;
      TouchScreen::y = SCREEN_HEIGHT - TouchScreen::y;
      TouchScreen::lastLowTime = now;
    }
  }
  else
  {
    Serial.println("HIGH");
    if ((now - TouchScreen::lastLowTime) > TouchScreen::debounceDelay)
    {
      Serial.println("untouched");
      TouchScreen::isTouched = false;
      TouchScreen::lastHighTime = now;
    }
  }
}
