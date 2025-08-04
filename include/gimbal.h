#ifndef GIMBAL_H
#define GIMBAL_H
#include "motor.h"

class Gimbal
{
private:

public:
    static Motor altitudeMotor;
    static Motor azimuthMotor;
    static void loop();
};

#endif
