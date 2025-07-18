#include <AccelStepper.h>

// Définition du type de driver (1 = DRIVER pour les pilotes avec STEP+DIR)
#define dirPin 14
#define stepPin 34

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void setup() {
  Serial.begin(115200);

  // Définir la vitesse maximale et l'accélération.
  stepper.setMaxSpeed(800);     // En pas par seconde
  stepper.setAcceleration(400);  // En pas par seconde^2

  // Définir une position cible
  stepper.moveTo(2000);  // Avancer de 2000 pas
}

void loop() {
  // Fait avancer le moteur vers la position cible
  stepper.run();

  // Une fois atteint, on recule
  if (stepper.distanceToGo() == 0) {
    stepper.moveTo(-stepper.currentPosition());  // Revenir à zéro
  }
}