## ADDED Requirements

### Requirement: Laser is off while gimbal motors are moving during calibration
During the calibration restoring-position phase, the laser SHALL be turned off before the gimbal motors are commanded to move, and SHALL remain off until both motors have stopped and the state transitions to `CALIBRATION_STATE_CALIBRATING`.

#### Scenario: Motors start moving toward calibration star
- **WHEN** the calibration view receives the target star coordinates and calls `goToAbsoluteAngle()` on both motors
- **THEN** the laser SHALL be off for the entire duration of that motor movement

#### Scenario: Laser state on arrival at calibration star
- **WHEN** both gimbal motors have finished moving and the state transitions to `CALIBRATION_STATE_CALIBRATING`
- **THEN** the laser state SHALL be controlled by the joystick Z-press, not automatically turned on
