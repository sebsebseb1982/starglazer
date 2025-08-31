# starglazer


## How to configure TFT_eSPI library

 - Copier [Setup21_ILI9488_seb.h](Setup21_ILI9488_seb.h) dans le dossier User_Setups de la librairie TFT_eSPI
 - Dans User_Setup_Select.h
   - Commenter #include <User_Setup.h>
   - Ajouter #include <User_Setups/Setup21_ILI9488_seb.h>

## Pinouts

Cf. [pinout.h](/include/pinout.h) and [TFT_eSPI configuration file](Setup21_ILI9488_seb.h).

### ESP32

|            | Peripheral     | Name               | Pin | Pin | Name     | Peripheral     |          |
|------------|----------------|--------------------|-----|-----|----------|----------------|----------|
|            |                |                    | 3V3 | GND |          |                |          |
|            |                |                    | EN  | 23  | TFT_MOSI | Screen         |          |
| Input only | Screen         | PEN (touch detect) | 36  | 22  | I2C_SCL  | WiiMote        |          |
| Input only |                |                    | 39  | 1   |          |                |          |
| Input only |                |                    | 34  | 3   |          |                |          |
| Input only | GPS            | TX                 | 35  | 21  | I2C_SDA  | WiiMote        |          |
|            | GPS            | RX                 | 32  | GND |          |                |          |
|            | Motor altitude | direction          | 33  | 19  | TFT_MISO | Screen         |          |
|            | Motor altitude | step               | 25  | 18  | TFT_SCLK | Screen         |          |
|            | Motor azimuth  | direction          | 26  | 5   | TFT_CS   | Screen         |          |
|            | Motor azimuth  | step               | 27  | 17  | TFT_DC   | Screen         |          |
|            | LED            | red                | 14  | 16  | TFT_RST  | Screen         |          |
|            | LED            | green              | 12  | 4   | BL       | Screen         |          |
|            |                |                    | GND | 0   | TOUCH_CS | Screen (touch) |          |
|            | LED            | blue               | 13  | 2   |          | Buzzer         |          |
| Reserved   |                |                    | 9   | 15  |          | Laser          |          |
| Reserved   |                |                    | 10  | 8   |          |                | Reserved |
| Reserved   |                |                    | 11  | 7   |          |                | Reserved |
| Reserved   |                |                    | 5V  | 6   |          |                | Reserved |

 ![Pinout Wroom devkit](/docs/images/doc-esp32-pinout-reference-wroom-devkit.jpg)

### RGB terminal block

TODO

### WiiMote extension cord

| Color  | Role    |
|--------|---------|
| white  | unused  |
| black  | I2C_SCL |
| red    | I2C_SDA |
| yellow | GND     |
| green  | 3.3v    |