## 1. Fix laser safety in CalibrationView

- [x] 1.1 In `src/views/calibration/calibration-view.cpp`, in `CALIBRATION_STATE_RESTORING_POSITION`: add `Laser::off()` immediately before the two `goToAbsoluteAngle()` calls to ensure the laser is off when motors start moving
- [x] 1.2 In the same state, remove the `Laser::on()` call that fires immediately after `goToAbsoluteAngle()` (the laser must stay off until the user enters fine-tuning mode)

## 2. Verify unchanged states remain correct

- [x] 2.1 Confirm `CALIBRATION_STATE_CALIBRATING` still turns the laser on/off via joystick Z-press (no change needed — verify only)
- [x] 2.2 Confirm `CALIBRATION_STATE_HOMING` has no `Laser::on()` call (no change needed — verify only)
