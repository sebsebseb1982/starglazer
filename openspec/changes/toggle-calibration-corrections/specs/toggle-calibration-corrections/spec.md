## ADDED Requirements

### Requirement: Calibration corrections can be toggled at runtime
The system SHALL provide a toggle mechanism to enable or disable the affine calibration corrections applied to API coordinates before driving the gimbal motors.

When disabled, `CalibrationData::applyCorrection()` SHALL return the raw API coordinates unchanged (identity), regardless of whether a calibration has been completed.

When enabled (and the system is calibrated), corrections SHALL be applied as before.

The toggle state SHALL default to **enabled** when entering the tracking view after a successful calibration.

#### Scenario: Corrections enabled (default)
- **WHEN** the user enters the tracking view after completing a 3-star calibration
- **THEN** calibration corrections are applied to API coordinates before driving the gimbal motors

#### Scenario: User disables corrections
- **WHEN** the user taps the calibration toggle button to disable corrections
- **THEN** the gimbal motors receive raw API coordinates on the next coordinate update cycle
- **AND** the toggle button shows an inactive indicator (gray LED)

#### Scenario: User re-enables corrections
- **WHEN** the user taps the calibration toggle button to re-enable corrections
- **THEN** the gimbal motors receive corrected coordinates on the next coordinate update cycle
- **AND** the toggle button shows an active indicator (green LED)

### Requirement: Calibration toggle button in tracking view
The tracking object view SHALL display a toggle button widget that controls the calibration correction enabled/disabled state.

The button SHALL use `GUI::drawToggleButton()` for consistent visual appearance with other toggle buttons (tracking, laser).

The button SHALL display an icon and the label "Calib." to identify its function.

#### Scenario: Button visual state reflects correction state
- **WHEN** calibration corrections are enabled
- **THEN** the button shows an ACTIVATED state (green LED)

#### Scenario: Button visual state when corrections disabled
- **WHEN** calibration corrections are disabled
- **THEN** the button shows a DESACTIVATED state (gray LED)

#### Scenario: Touch interaction toggles state
- **WHEN** the user touches and releases the calibration toggle button
- **THEN** the correction enabled state is inverted (enabled→disabled or disabled→enabled)
- **AND** the button redraws immediately to reflect the new state
