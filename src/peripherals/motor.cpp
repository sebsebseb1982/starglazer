#include "motor.h"

Motor::Motor(int dirPin,
             int stepPin,
             int stepsPerRevolution,
             int enabledPin,
             String label) : stepsPerRevolution(stepsPerRevolution), label(label)
{
    this->stepper = AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
    this->stepper.setMaxSpeed(1000);    // vit. max en pas/sec
    this->stepper.setAcceleration(500); // acc. en pas/sec²
    pinMode(enabledPin, OUTPUT);
    digitalWrite(enabledPin, LOW);
}

void Motor::goToAbsoluteAngle(float angleInDegrees)
{
    long destinationSteps = (angleInDegrees / 360.0) * stepsPerRevolution;
    long moveValue = destinationSteps - this->stepper.currentPosition();
    String message;
    message += F("Moving motor ");
    message += label;
    message += F(" ");
    message += moveValue;
    message += F(" step(s) absoluteAngle = ");
    message += angleInDegrees;
    message += F(" destinationSteps = ");
    message += destinationSteps;
    message += F(" currentPosition = ");
    message += this->stepper.currentPosition();
    Serial.println(message);
    this->stepper.move(moveValue);
}

void Motor::loop()
{
    if (this->stepper.distanceToGo() != 0)
    {
        this->stepper.run();
    }
}