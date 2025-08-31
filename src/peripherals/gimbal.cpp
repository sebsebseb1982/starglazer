#include "gimbal.h"
#include "pinout.h"

Motor Gimbal::altitudeMotor = Motor(MOTOR_ALTITUDE_DIRECTION_PIN,
                                MOTOR_ALTITUDE_STEP_PIN,
                                3200,
                                "Altitude");
Motor Gimbal::azimuthMotor = Motor(MOTOR_AZIMUTH_DIRECTION_PIN,
                               MOTOR_AZIMUTH_STEP_PIN,
                               3200,
                               "Azimuth");

void Gimbal::loop()
{
    Gimbal::altitudeMotor.loop();
    Gimbal::azimuthMotor.loop();
}
