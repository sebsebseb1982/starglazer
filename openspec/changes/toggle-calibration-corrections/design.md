## Context

The tracking object view drives gimbal motors using coordinates fetched from the API, corrected by affine coefficients computed during a 3-star calibration. The correction is applied inside `TrackingObjectService::loop()` via `CalibrationData::applyCorrection()`. There is no runtime control to bypass this correction — it is always applied when `CalibrationData::calibrated` is `true`.

The tracking view currently has 8 widgets in a 6×4 grid (480×320 display, 80px cells). Several grid positions remain unused and available for a new button.

## Goals / Non-Goals

**Goals:**
- Allow the user to toggle calibration corrections on/off from the tracking view via a touch button.
- Default to corrections enabled after a successful calibration.
- The toggle must take effect immediately on the next coordinate update cycle.

**Non-Goals:**
- Persisting the toggle state across view transitions or reboots.
- Providing partial correction (e.g., altitude only). It's all or nothing.
- Modifying the calibration workflow itself.

## Decisions

### 1. Add a static `enabled` flag on `CalibrationData`

**Decision**: Add `static bool enabled` to `CalibrationData`, initialized to `true`. `applyCorrection()` will check both `calibrated && enabled` before applying coefficients. When `enabled` is `false`, it returns identity (raw coordinates pass through).

**Rationale**: This keeps the toggle logic centralized in the existing correction path. The alternative — adding a flag in `TrackingObjectService` and branching around the `applyCorrection()` call — would scatter the concern across two classes. Since `CalibrationData` already owns `calibrated`, adding `enabled` beside it is cohesive.

### 2. New toggle button widget: `WidgetCalibrationToggleButton`

**Decision**: Create a new widget following the exact same pattern as `WidgetTrackingButton` and `WidgetLaserButton` — a toggle button using `GUI::drawToggleButton()` with an icon.

**Rationale**: The codebase has an established toggle button pattern. Reusing it keeps the UI consistent and the implementation simple.

### 3. Grid placement: column 3, row 2

**Decision**: Place the new widget at grid position (3, 2) — currently unused, adjacent to the existing "Calibrate" button at (3, 3).

**Rationale**: Grouping calibration-related controls keeps the layout logical. The position is available and doesn't conflict with existing widgets.

### 4. Use the compass icon (`image-compass.h`)

**Decision**: Use the existing compass icon for this toggle button.

**Rationale**: The compass icon conveys alignment/correction visually. It already exists in the codebase, so no new asset is needed.

## Risks / Trade-offs

- **[Risk] User disables corrections and forgets** → The button has a clear LED indicator (green = on, gray = off) consistent with other toggle buttons. The label "Calib." provides context.
- **[Risk] Additional widget increases per-loop iteration time** → Negligible; the widget only reads a static bool, no I/O or computation. The refresh period can be set to `HUNDRED_MILLISECONDS` matching other toggle buttons.
