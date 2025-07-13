
#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "buttons.h"

#define BUTTON_SIZE 80
#define BUTTON_MARGIN 2
#define ICON_SIZE 32
#define LED_MARGIN 10
#define LED_SIZE 4

class GUI {
  private:
    TFT_eSPI *screen;
  public:
    GUI();
    GUI(TFT_eSPI *screen);

    void drawButton(
      unsigned int positionX,
      unsigned int positionY,
      unsigned int sizeX,
      unsigned int sizeY,
      String label,
      ButtonStatus buttonStatus
    );

    void drawToggleButton(
      unsigned int x,
      unsigned int y,
      String label,
      ButtonStatus buttonStatus
    );
};

#endif
