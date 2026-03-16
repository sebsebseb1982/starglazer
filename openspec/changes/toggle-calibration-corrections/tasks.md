## 1. CalibrationData — add `enabled` flag

- [x] 1.1 Add `static bool enabled` declaration to `include/calibration-data.h`
- [x] 1.2 Initialize `CalibrationData::enabled = true` in `src/views/calibration/calibration-data.cpp` and reset it to `true` in `CalibrationData::reset()`
- [x] 1.3 Update `CalibrationData::applyCorrection()` to check `calibrated && enabled` — return identity when either is `false`

## 2. Widget — create `WidgetCalibrationToggleButton`

- [x] 2.1 Create `include/widget-calibration-toggle-button.h` with the widget class declaration following the `WidgetTrackingButton` pattern (value, previousValue, buttonStatus fields; draw/refreshValue/isValueChanged/manageTouchUp/manageTouchDown methods)
- [x] 2.2 Create `src/views/tracking/ui/widgets/widget-calibration-toggle-button.cpp` implementing the widget: `drawToggleButton` with compass icon, toggle `CalibrationData::enabled` on touch up

## 3. TrackingObjectView — integrate the new widget

- [x] 3.1 Update `#define widgetNumbers` from 8 to 9 in `include/tracking-object-view.h`
- [x] 3.2 Add `#include "widget-calibration-toggle-button.h"` and instantiate the widget at grid position (3, 2) with label "Calib." in `src/views/tracking/ui/tracking-object-view.cpp`

## 4. Verification

- [x] 4.1 Build the project with `pio run` and confirm no compilation errors
