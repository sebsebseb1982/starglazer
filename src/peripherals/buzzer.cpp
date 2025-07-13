#include "buzzer.h"

#define pwmChannel 1 //Choisit le canal 0
#define frequence 1000 //Fréquence PWM de 1 KHz
#define resolution 8 // Résolution de 8 bits, 256 valeurs possibles

#define BUZZER_PIN 13

unsigned long Buzzer::stopBeepMillis = 0;
unsigned int Buzzer::currentLevel = 0;

void Buzzer::setup() {
  ledcSetup(pwmChannel, frequence, resolution);
  ledcAttachPin(BUZZER_PIN, pwmChannel);
  ledcWrite(pwmChannel, currentLevel);
}

void Buzzer::on() {
  ledcWrite(
    pwmChannel,
    currentLevel
  );
}

void Buzzer::off() {
  ledcWrite(
    pwmChannel,
    0
  );
}

void Buzzer::beep(unsigned int durationInMs) {
  on();
  Buzzer::stopBeepMillis = millis() + durationInMs;
}

void Buzzer::refresh() {

    currentLevel = 3;

  if (millis() > stopBeepMillis) {
    off();
  }
}
