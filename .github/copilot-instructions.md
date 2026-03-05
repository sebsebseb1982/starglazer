# Starglazer — Project Guidelines

ESP32 Arduino/PlatformIO star-tracking gimbal with ILI9488 touchscreen, GPS, stepper motors, and a companion API.

## Code Style

- **Files**: `kebab-case` (`tracking-object-service.h`)
- **Classes**: `PascalCase` (`TrackingObjectService`)
- **Methods/variables**: `camelCase` (`refreshValue()`, `trackedObject`)
- **Constants/macros**: `UPPER_SNAKE_CASE` (`SCREEN_WIDTH`, `FIVE_SECONDS`)
- **Header guards**: `#ifndef VIEW_H` / `#define VIEW_H` / `#endif` (not `#pragma once`)
- **Includes**: quoted for project (`"screen.h"`), angle brackets for libraries (`<TFT_eSPI.h>`)
- Headers go in `include/`, implementations in `src/` subdirectories organized by feature

## Architecture

**Dual-core ESP32** — Core 0 runs UI/input loop (joystick, screen, touch, view service); Core 1 runs GPS and WiFi. Entry point: [src/main.cpp](src/main.cpp).

**View system** — Abstract `View` base class with `setup()`/`loop()`. Navigation via `CurrentViewService::changeCurrentView(new SomeView(...))` which calls `setup()`, deletes the old view, and stores the pointer. Flow: `SplashScreenView` → `CalibrationView` → `ChoosingObjectView` → `TrackingObjectView`.

**Widget system** — Abstract `Widget` base with `draw()`, `refreshValue()`, `isValueChanged()`. Grid-based layout: 80px cells, 6 columns × 4 rows on 480×320 display. Views create widgets with `new` in `setup()`, store in array or `std::list<Widget*>`, iterate `refresh()` in `loop()`, and `delete` in destructor.

**Peripherals** — Static singleton pattern: all-static classes with `setup()`/`loop()` called via `ClassName::method()` (e.g. `GPS::loop()`, `Laser::on()`). See [include/pinout.h](include/pinout.h) for GPIO assignments.

**Tracking** — `TrackingObjectService` polls the remote API every 5s via `EquatorialCoordinatesService::compute()` to get altitude/azimuth, then drives the gimbal motors.

## Build and Test

```bash
# Build
pio run

# Upload to ESP32 (COM6)
pio run --target upload

# Serial monitor (115200 baud)
pio device monitor
```

Board: `esp32dev` | Framework: `arduino` | Config: [platformio.ini](platformio.ini)

Key libraries: `TFT_eSPI`, `TinyGPSPlus`, `ArduinoJson`, `AccelStepper`, `WiiChuck`, `UrlEncode`

## Project Conventions

- **Widget headers** are in `include/widget-*.h` with full class declaration; `.cpp` implementations live under the owning view's directory (e.g. `src/views/calibration/widgets/`, `src/views/tracking/ui/`)
- **Debug logging** uses `DEBUG_PRINT()`/`DEBUG_PRINTLN()` macros from [include/debug.h](include/debug.h), gated by `DEBUG_SERIAL`
- **Timing constants** in [include/duration.h](include/duration.h): `HUNDRED_MILLISECONDS`, `ONE_SECOND`, `FIVE_SECONDS` — use these instead of raw millisecond values
- **Colors** defined as RGB565 macros in [include/colors.h](include/colors.h)
- **Memory management** uses raw `new`/`delete` — views own and delete their widgets; `CurrentViewService` deletes old views on transition
- **GUI drawing helpers** `Gui::drawButton()` / `Gui::drawToggleButton()` in [src/widgets/gui.cpp](src/widgets/gui.cpp) for consistent button rendering
- **API calls** use HTTP GET with retry loop (max 10 attempts), `ArduinoJson` deserialization, 2s connect / 5s read timeouts — see [src/api/starglaze-api.cpp](src/api/starglaze-api.cpp)

## Integration Points

- **Starglaze API** — REST backend at `STARGLAZE_API_HOST` (defined in [include/secrets.h](include/secrets.h)). Endpoints: `GET /catalog` (object list), `GET /horizontal-coordinates/{type}/{code}?latitude=&longitude=&elevation=` (coordinate conversion)
- **WiFi** — auto-connects to `WIFI_SSID`/`WIFI_PASSWORD` from `secrets.h`; auto-restarts ESP32 after 20 failed reconnect attempts
- **GPS** — UART on Serial2 (RX=35, TX=32), provides lat/lng/elevation via `GPS::data`
- **TFT display** — ILI9488 over SPI with custom setup in [Setup21_ILI9488_seb.h](Setup21_ILI9488_seb.h); must be copied into TFT_eSPI library's `User_Setups/` folder

## Security

- **Secrets** in [include/secrets.h](include/secrets.h) — contains WiFi credentials and API host. Do not commit real values.
