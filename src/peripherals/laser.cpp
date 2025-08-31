#include <Arduino.h>
#include "laser.h"
#include "pinout.h"

void Laser::off()
{
    pinMode(LASER_PIN, OUTPUT);
    digitalWrite(LASER_PIN, LOW);
}

void Laser::on()
{
    pinMode(LASER_PIN, OUTPUT);
    digitalWrite(LASER_PIN, HIGH);
}