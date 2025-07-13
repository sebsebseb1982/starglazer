#include <TFT_eSPI.h>
#include "touch-screen.h"
#include "screen.h"

boolean TouchScreen::isTouched = false;
uint16_t TouchScreen::x = 0;
uint16_t TouchScreen::y = 0;

void TouchScreen::refresh(TFT_eSPI *screen) {
  TouchScreen::isTouched = screen->getTouch(&TouchScreen::x, &TouchScreen::y);
  TouchScreen::x = SCREEN_WIDTH - TouchScreen::x;
  TouchScreen::y = SCREEN_HEIGHT - TouchScreen::y;
}
