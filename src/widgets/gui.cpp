#include "gui.h"
#include "colors.h"

GUI::GUI() {}

GUI::GUI(TFT_eSPI *screen) {
  this->screen = screen;
}

void GUI::drawButton(
  unsigned int positionX,
  unsigned int positionY,
  unsigned int sizeX,
  unsigned int sizeY,
  String label,
  ButtonStatus buttonStatus
) {
  if (buttonStatus == ACTIVATED) {
    screen->fillRoundRect(
      positionX + BUTTON_MARGIN,
      positionY + BUTTON_MARGIN,
      sizeX * BUTTON_SIZE - (2 * BUTTON_MARGIN),
      sizeY * BUTTON_SIZE - (2 * BUTTON_MARGIN),
      LED_MARGIN - 1,
      LIGHT_GRAY
    );
  } else {
    screen->fillRoundRect(
      positionX + BUTTON_MARGIN,
      positionY + BUTTON_MARGIN,
      sizeX * BUTTON_SIZE - (2 * BUTTON_MARGIN),
      sizeY * BUTTON_SIZE - (2 * BUTTON_MARGIN),
      LED_MARGIN,
      BACKGROUND_COLOR
    );
  }

  uint16_t buttonBorderColor;
  if (buttonStatus == UNAVAILABLE || buttonStatus == ACTIVATED) {
    buttonBorderColor = DARK_GRAY;
  } else {
    buttonBorderColor = WHITE;
  }

  screen->drawRoundRect(
    positionX + BUTTON_MARGIN,
    positionY + BUTTON_MARGIN,
    sizeX * BUTTON_SIZE - (2 * BUTTON_MARGIN),
    sizeY * BUTTON_SIZE - (2 * BUTTON_MARGIN),
    LED_MARGIN,
    buttonBorderColor
  );

  screen->drawString(
    label,
    positionX + (sizeX * BUTTON_SIZE / 2) + BUTTON_MARGIN,
    positionY + sizeY * BUTTON_SIZE - 17,
    1
  );
}

void GUI::drawToggleButton(
  unsigned int x,
  unsigned int y,
  String label,
  ButtonStatus buttonStatus
) {
  drawButton(
    x,
    y,
    1,
    1,
    label,
    buttonStatus
  );
  int delta = 2;
  if (buttonStatus == ACTIVATED) {
    screen->fillCircle(
      x + BUTTON_SIZE - BUTTON_MARGIN - LED_MARGIN - delta,
      y + BUTTON_MARGIN + LED_MARGIN + delta,
      LED_SIZE,
      LED_GREEN_COLOR
    );
  } else {
    screen->fillCircle(
      x + BUTTON_SIZE - BUTTON_MARGIN - LED_MARGIN - delta,
      y + BUTTON_MARGIN + LED_MARGIN + delta,
      LED_SIZE,
      LIGHT_GRAY
    );
  }
}
