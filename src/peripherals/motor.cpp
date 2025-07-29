#include "motor.h"
#include <AccelStepper.h>

void Motor::turn()
{
    AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

    // Définir la vitesse maximale et l'accélération.
    stepper.setMaxSpeed(3000);     // En pas par seconde
    stepper.setAcceleration(1000); // En pas par seconde^2

    stepper.moveTo(5000); // Avancer de 2000 pas

    while (stepper.distanceToGo() != 0)
    {
        stepper.run();
    }
}