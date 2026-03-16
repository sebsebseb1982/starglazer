## Context

`CalibrationView` uses a state machine to guide the user through pointing the gimbal at calibration stars. In `CALIBRATION_STATE_RESTORING_POSITION` the view fetches the star's horizontal coordinates, commands both stepper motors to move to the target position, and then waits for movement to finish before switching to `CALIBRATION_STATE_CALIBRATING`. The bug: `Laser::on()` is called at the same time as `goToAbsoluteAngle()`, so the laser fires while the gimbal is still sweeping to its target. The same issue exists in `CALIBRATION_STATE_HOMING`.

## Goals / Non-Goals

**Goals:**
- The laser SHALL be off for the entire duration that gimbal motors are in motion during the calibration flow.
- The fix SHALL be self-contained inside `CalibrationView`'s state machine — no changes to peripheral classes.

**Non-Goals:**
- Adding a general "motor moving → laser off" guard inside `Laser`, `Motor`, or `Gimbal` — too much coupling for a targeted safety fix.
- Changing the manual fine-tuning behavior in `CALIBRATION_STATE_CALIBRATING` (laser already correctly follows joystick Z-press there).
- Modifying the tracking flow (`TrackingObjectService`) — it has its own laser logic and is not part of calibration.

## Decisions

**Remove the premature `Laser::on()` in `CALIBRATION_STATE_RESTORING_POSITION`**

When coordinates arrive and motors are commanded to move, the existing code calls `Laser::on()` immediately. This line must be deleted. The laser will instead be turned on as part of the entry into `CALIBRATION_STATE_CALIBRATING` (which already owns laser-on-for-z-press behavior), or at a dedicated entry action when transitioning into that state.

*Alternative considered*: Add a guard in `Motor::loop()` to cut the laser if moving. Rejected — it creates unexpected coupling between Motor and Laser, and makes the laser behaviour implicit and hard to audit.

**Ensure `Laser::off()` is explicitly called at the start of each motor-movement phase**

In `CALIBRATION_STATE_RESTORING_POSITION`, call `Laser::off()` once before issuing `goToAbsoluteAngle()`, to handle the edge case where the laser could have been left on (e.g. a previous state transition that didn't clean up).

**No change required for `CALIBRATION_STATE_HOMING`**

`Laser::off()` is already called when `CalibrationView` leaves `CALIBRATION_STATE_CALIBRATING` (on `calibrationDone`). The homing state never turns the laser on, so it is already safe.

## Risks / Trade-offs

- [Risk] Removing `Laser::on()` from the restoring-position phase could leave the user confused if they expect the laser to already be on when the gimbal arrives → Mitigation: the laser is turned on when the user enters fine-tuning mode (`CALIBRATION_STATE_CALIBRATING`), which is the correct moment anyway.
- [Risk] Very small surface change; risk of regression is low → no additional mitigation needed.
