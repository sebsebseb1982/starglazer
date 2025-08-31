#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <WiiChuck.h>

#define RAW_MIDDLE_VALUE 128
#define DEADZONE_VALUE 3

class JoystickDirection
{
private:

public:
    JoystickDirection();
    JoystickDirection(
        boolean pressed,
        int rawValue);
    boolean pressed;
    float value;
};

class JoystickStatus
{
private:
public:
    JoystickStatus();
    JoystickStatus(
        JoystickDirection left,
        JoystickDirection right,
        JoystickDirection up,
        JoystickDirection down,
        boolean cPressed,
        boolean zPressed);
    JoystickDirection left;
    JoystickDirection right;
    JoystickDirection up;
    JoystickDirection down;
    boolean cPressed;
    boolean zPressed;
    String toString();
};

class Joystick
{
private:
    static Accessory instance;

public:
    static JoystickStatus status;
    static void setup();
    static void loop();
};

#endif
