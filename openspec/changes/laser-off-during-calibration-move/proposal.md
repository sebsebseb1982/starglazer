## Why

During calibration, the gimbal motors move automatically to point at a target star. Currently the laser is switched on at the same moment the motors begin this movement, meaning the laser beam sweeps uncontrolled across the environment until the motors stop. This is a safety hazard and must be corrected.

## What Changes

- The laser must be kept **off** while the gimbal motors are moving toward the target star during `CALIBRATION_STATE_RESTORING_POSITION`.
- The laser is turned on only after both motors have reached their target (i.e., when the state transitions to `CALIBRATION_STATE_CALIBRATING`).
- The same safety rule applies during the `CALIBRATION_STATE_HOMING` phase (motors moving back to home): the laser must remain off for the full duration of homing.

## Capabilities

### New Capabilities
- `laser-motor-safety`: Guarantee that the laser is always off whenever the gimbal motors are in motion during the calibration flow.

### Modified Capabilities
<!-- No existing spec-level requirements are changing (openspec/specs/ is currently empty). -->

## Impact

- `src/views/calibration/calibration-view.cpp` — Remove the premature `Laser::on()` call in the restoring-position state; ensure `Laser::off()` is enforced at the start of every motor-movement phase.
- No changes to `Laser`, `Motor`, `Gimbal`, or any other peripheral class — the fix is purely in the calibration view's state machine.
