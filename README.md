# starglazer


## How to configure TFT_eSPI library

 - Copier Setup21_ILI9488_seb.h dans le dossier User_Setups de la librairie TFT_eSPI
 - Dans User_Setup_Select.h
   - Commenter #include <User_Setup.h>
   - Ajouter #include <User_Setups/Setup21_ILI9488_seb.h>

## Pinouts

### ESP32

|            | Peripheral | Name               | Pin | Pin | Name     | Peripheral     |          |
|------------|------------|--------------------|-----|-----|----------|----------------|----------|
|            |            |                    | 3V3 | GND |          |                |          |
|            |            |                    | EN  | 23  | TFT_MOSI | Screen         |          |
| Input only |            |                    | 36  | 22  |          |                |          |
| Input only |            |                    | 39  | 1   |          |                |          |
| Input only |            |                    | 34  | 3   |          |                |          |
| Input only |            |                    | 35  | 21  | TOUCH_CS | Screen (touch) |          |
|            |            |                    | 32  | GND |          |                |          |
|            |            |                    | 33  | 19  | TFT_MISO | Screen         |          |
|            |            |                    | 25  | 18  | TFT_SCLK | Screen         |          |
|            | Screen     | BL                 | 26  | 5   | TFT_CS   | Screen         |          |
|            | Screen     | PEN (touch detect) | 27  | 17  | TFT_DC   | Screen         |          |
|            |            |                    | 14  | 16  | TFT_RST  | Screen         |          |
|            |            |                    | 12  | 4   | RX       | GPS            |          |
|            |            |                    | GND | 0   | TX       | GPS            |          |
|            | Buzzer     |                    | 13  | 2   |          |                |          |
|            |            |                    | 9   | 15  |          |                |          |
| Reserved   |            |                    | 10  | 8   |          |                | Reserved |
| Reserved   |            |                    | 11  | 7   |          |                | Reserved |
| Reserved   |            |                    | 5V  | 6   |          |                | Reserved |

 ![Pinout Wroom devkit](/docs/images/doc-esp32-pinout-reference-wroom-devkit.jpg)

Cf. [pinout.h](/src/include/pinout.h).

### RGB terminal block

 - Ecran + tactile
  - CS 5
  - RST 16
  - D/C 17
  - SDI 23
  - SCK 18
  - BL 26
  - SDO Pas branché
 - 2 moteurs pas à pas + detecteur HOME ?
 - Manette Wii
 - Laser
 - GPS
 - Buzzer
 - Led en dessous de l'horizon

### WiiMote extension cord

| Color  | Role    |
|--------|---------|
| white  | unused  |
| black  | I2C_SCL |
| red    | I2C_SDA |
| yellow | GND     |
| green  | 3.3v    |