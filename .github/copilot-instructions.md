# Project Guidelines

## Code Style
- Use modern C++ (classes, RAII, smart pointers where possible)
- Prefer explicit initialization and clear naming (see src/main.cpp, include/motor.h)
- UI logic is split into View classes (see include/view.h, src/views/*)
- Pin definitions and hardware mappings are centralized in include/pinout.h
- Use `String` for textual data, avoid raw char arrays
- Widget and View classes use lists for dynamic UI elements

## Architecture
- Main entry: src/main.cpp
- Hardware abstraction: src/peripherals/* (Motor, Gimbal, GPS, Joystick, Laser, RGB LED, WiFi)
- UI: src/views/* (SplashScreenView, CalibrationView, ChoosingObjectView, TrackingObjectView)
- API integration: src/api/starglaze-api.cpp (uses HTTPClient, ArduinoJson)
- Pinout and hardware config: include/pinout.h, Setup21_ILI9488_seb.h
- Views transition via CurrentViewService (see include/current-view-service.h)
- Motor control via AccelStepper (see include/motor.h, src/peripherals/motor.cpp)

## Build and Test
- Build with PlatformIO (see platformio.ini)
  - Board: esp32dev
  - Framework: arduino
  - Build: `pio run`
  - Upload: `pio run --target upload`
  - Monitor: `pio device monitor`
- Required libraries: TFT_eSPI, TinyGPSPlus, ArduinoJson, AccelStepper, UrlEncode, WiiChuck
- Build flags: `-Isrc/include/`
- Exception decoder enabled for ESP32

## Project Conventions
- All hardware pins defined in include/pinout.h
- TFT_eSPI setup: copy Setup21_ILI9488_seb.h to TFT_eSPI/User_Setups, update User_Setup_Select.h
- Views must implement setup() and loop() (see include/view.h)
- Widget classes for UI buttons/status (see include/widget.h, src/widgets/)
- API host and WiFi credentials in include/secrets.h
- Use static members for global state (see CalibrationView, ChoosingObjectView, TrackingObjectService)

## Integration Points
- External API: STARGLAZE_API_HOST (see include/secrets.h, src/api/starglaze-api.cpp)
- WiFi: WiFiConnection class
- GPS: GPS class
- Screen: TFT_eSPI

## Security
- WiFi credentials and API host are hardcoded in include/secrets.h (do not commit secrets to public repos)
- No authentication for API calls (HTTP only)
- Sensitive operations (laser, motors) are controlled via UI and joystick
