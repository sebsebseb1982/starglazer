#include "gimbal.h"

Motor Gimbal::altitudeMotor = Motor(33,
                                25,
                                3200,
                                32,
                                "Altitude");
Motor Gimbal::azimuthMotor = Motor(27,
                               14,
                               3200,
                               32,
                               "Azimuth");

void Gimbal::loop()
{
    Gimbal::altitudeMotor.loop();
    Gimbal::azimuthMotor.loop();
}
