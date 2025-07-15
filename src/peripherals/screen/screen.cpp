#include "screen.h"
#include "colors.h"

#define pwmChannel 0    //Choisit le canal 0
#define frequence 1000  //Fréquence PWM de 1 KHz
#define resolution 8    // Résolution de 8 bits, 256 valeurs possibles
#define pwmPin 26

Screen::Screen(TFT_eSPI *screen) {
  this->screen = screen;
  this->lastTouchedMillis = millis();
}

void Screen::setup() {
  screen->init();
  screen->begin();
  screen->fillScreen(BACKGROUND_COLOR);
  screen->setRotation(1);
  screen->setTextColor(WHITE, BACKGROUND_COLOR);
  screen->setTextDatum(4);
  //ledcAttachChannel(pwmPin, frequence, resolution, pwmChannel);
  //ledcWrite(pwmChannel, 255);
  pinMode(pwmPin, OUTPUT);
  digitalWrite(pwmPin, HIGH);
}

void Screen::loop() {
}
