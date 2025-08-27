#include "joystick.h"

JoystickDirection::JoystickDirection() {};

JoystickStatus::JoystickStatus() {};

JoystickDirection::JoystickDirection(
	boolean pressed,
	int rawValue) : pressed(pressed)
{
	this->value = (rawValue * 1.0) / (RAW_MIDDLE_VALUE * 1.0);
};

JoystickStatus::JoystickStatus(
	JoystickDirection left,
	JoystickDirection right,
	JoystickDirection up,
	JoystickDirection down,
	boolean cPressed,
	boolean zPressed) : left(left), right(right), up(up), down(down), cPressed(cPressed), zPressed(zPressed) {}

String JoystickStatus::toString()
{
	String message;
	message += F("up    value=");
	message += up.value;
	message += F(" status=");
	message += up.pressed ? "pressed" : "";
	message += F("\n");

	message += F("down  value=");
	message += down.value;
	message += F(" status=");
	message += down.pressed ? "pressed" : "";
	message += F("\n");

	message += F("left  value=");
	message += left.value;
	message += F(" status=");
	message += left.pressed ? "pressed" : "";
	message += F("\n");

	message += F("right value=");
	message += right.value;
	message += F(" status=");
	message += right.pressed ? "pressed" : "";
	message += F("\n");

	message += F("c status=");
	message += cPressed ? "pressed" : "";
	message += F("\n");

	message += F("z status=");
	message += zPressed ? "pressed" : "";

	return message;
}

JoystickStatus Joystick::status = JoystickStatus();
Accessory Joystick::instance;

void Joystick::setup()
{
	Joystick::instance.begin();
	if (Joystick::instance.type == UnknownChuck)
	{
		/** If the device isn't auto-detected, set the type explicatly
		 * 	NUNCHUCK,
		 WIICLASSIC,
		 GuitarHeroController,
		 GuitarHeroWorldTourDrums,
		 DrumController,
		 DrawsomeTablet,
		 Turntable
		 */
		Joystick::instance.type = NUNCHUCK;
	}
}

void Joystick::loop()
{
	Joystick::instance.readData();

	uint8_t horizontalValue = (uint8_t)Joystick::instance.values[0];
	uint8_t verticalValue = (uint8_t)Joystick::instance.values[1];
	uint8_t cValue = (uint8_t)Joystick::instance.values[11];
	uint8_t zValue = (uint8_t)Joystick::instance.values[10];

	/*
	String horizontalValueMessage = F("horizontalValue=");
	horizontalValueMessage += horizontalValue;
	horizontalValueMessage += F("\n");
	Serial.println(horizontalValue);

	String verticalValueMessage = F("verticalValue=");
	verticalValueMessage += verticalValue;
	Serial.println(verticalValueMessage);
	*/

	// TODO Normaliser les valeurs entre 0 et 128
	int leftValue = -1 * (horizontalValue - RAW_MIDDLE_VALUE);
	int rightValue = horizontalValue - RAW_MIDDLE_VALUE;
	int upValue = verticalValue - RAW_MIDDLE_VALUE;
	int downValue = -1 * (verticalValue - RAW_MIDDLE_VALUE);

	Joystick::status = JoystickStatus(
		JoystickDirection(
			leftValue > DEADZONE_VALUE,
			leftValue),
		JoystickDirection(
			rightValue > DEADZONE_VALUE,
			rightValue),
		JoystickDirection(
			upValue > DEADZONE_VALUE,
			upValue),
		JoystickDirection(
			downValue > DEADZONE_VALUE,
			downValue),
		cValue > RAW_MIDDLE_VALUE,
		zValue > RAW_MIDDLE_VALUE);
}
