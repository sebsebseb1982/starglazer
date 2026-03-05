#include "motor.h"
#include "debug.h"

Motor::Motor(int dirPin,
             int stepPin,
             int stepsPerRevolution,
             String label) : stepsPerRevolution(stepsPerRevolution),
                             label(label),
                             mux(portMUX_INITIALIZER_UNLOCKED),
                             homingInProgress(false),
                             homingComplete(false)
{
    this->stepper = AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
    this->stepper.setMaxSpeed(4000);
    this->stepper.setAcceleration(2000);
}

void Motor::goToAbsoluteAngle(float angleInDegrees)
{
    float motorAngleInDegrees = angleInDegrees < -180.0 ? (360.0 + angleInDegrees) : angleInDegrees;
    long destinationSteps = (motorAngleInDegrees / 360.0) * stepsPerRevolution;
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
    DEBUG_PRINTLN(message);
    portENTER_CRITICAL(&mux);
    this->stepper.move(moveValue);
    portEXIT_CRITICAL(&mux);
}

void Motor::rotateNSteps(long steps)
{
    portENTER_CRITICAL(&mux);
    this->stepper.move(steps);
    portEXIT_CRITICAL(&mux);
}

void Motor::goToHome(float homeAngleInDegrees)
{
    portENTER_CRITICAL(&mux);
    this->stepper.setCurrentPosition(0);
    portEXIT_CRITICAL(&mux);
    homingComplete = false;
    homingInProgress = true;
    goToAbsoluteAngle(homeAngleInDegrees);
}

bool Motor::isMoving()
{
    portENTER_CRITICAL(&mux);
    bool moving = this->stepper.distanceToGo() != 0;
    portEXIT_CRITICAL(&mux);
    return moving;
}

void Motor::loop()
{
    portENTER_CRITICAL(&mux);
    if (this->stepper.distanceToGo() != 0)
    {
        this->stepper.run();
    }
    bool reachedTarget = homingInProgress && (this->stepper.distanceToGo() == 0);
    portEXIT_CRITICAL(&mux);

    if (reachedTarget)
    {
        portENTER_CRITICAL(&mux);
        this->stepper.setCurrentPosition(0);
        portEXIT_CRITICAL(&mux);
        homingInProgress = false;
        homingComplete = true;
    }
}