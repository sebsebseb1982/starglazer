## Why

When tracking an object, the gimbal applies affine corrections computed during the 3-star calibration process. There is currently no way to disable these corrections at runtime. Being able to toggle corrections on/off is useful for diagnosing tracking errors — the user can quickly compare raw API coordinates vs. corrected ones to determine whether the calibration itself is degrading accuracy (e.g., after a long session where drift has accumulated).

## What Changes

- Add a new toggle button widget in the tracking object view that enables or disables the calibration corrections applied by `CalibrationData::applyCorrection()`.
- When corrections are disabled, the tracking service sends raw API altitude/azimuth directly to the gimbal motors (identity pass-through), just as if `CalibrationData::calibrated` were `false`.
- The toggle state defaults to **enabled** (corrections applied) when entering the tracking view after a successful calibration.

## Capabilities

### New Capabilities
- `toggle-calibration-corrections`: A toggle button widget in the tracking view that lets the user enable/disable the affine corrections computed during 3-star calibration.

### Modified Capabilities
<!-- No existing specs to modify -->

## Impact

- **Code**: `CalibrationData` (new `enabled` flag), `TrackingObjectService` (respect the flag), new widget header + implementation, `TrackingObjectView` (add widget to layout).
- **Layout**: One additional button occupies a grid cell in the tracking view (currently 8 widgets, will become 9).
- **Dependencies**: None — uses existing `GUI::drawToggleButton()` and existing calibration infrastructure.
