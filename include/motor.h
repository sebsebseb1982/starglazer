
#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <AccelStepper.h>

class Motor
{
private:
  int stepsPerRevolution;
  String label;
  AccelStepper stepper;

public:
  Motor(int dirPin,
        int stepPin,
        int stepsPerRevolution,
        int enabledPin,
        String label);
  void goToAbsoluteAngle(float angleInDegrees);
  void rotateNSteps(long steps);
  void goToHome(float homeAngleInDegrees);
  void loop();
};

#endif
