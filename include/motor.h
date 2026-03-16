
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
  portMUX_TYPE mux;
  bool homingInProgress;

public:
  bool homingComplete;
  Motor(int dirPin,
        int stepPin,
        int stepsPerRevolution,
        String label);
  void goToAbsoluteAngle(float angleInDegrees);
  void rotateNSteps(long steps);
  // Non-blocking: sets target and returns immediately.
  // Check homingComplete flag to know when done.
  void goToHome(float homeAngleInDegrees);
  float getCurrentAngle();
  bool isMoving();
  void loop();
};

#endif
