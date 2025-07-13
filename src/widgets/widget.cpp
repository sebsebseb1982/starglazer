#include "widget.h"
#include "screen.h"
#include "touch-screen.h"

Widget::Widget(
  unsigned int column,
  unsigned int row,
  String label,
  TFT_eSPI *screen,
  unsigned long refreshPeriodInMs
) {
  this->x = column * BUTTON_SIZE;
  this->y = row * BUTTON_SIZE;
  this->label = label;
  this->screen = screen;
  this->refreshPeriodInMs = refreshPeriodInMs;
  this->gui = (screen);
  this->startMillis = millis();
}

void Widget::init() {
  Serial.println("Widget::init()");
  refreshValue();
  draw();
}

void Widget::refresh() {
  Serial.println("Widget::refresh() start");
  currentMillis = millis();
  boolean valueRefreshed = false;
  unsigned long refreshValueDurationInMs = 0;
  Serial.println(currentMillis - startMillis);
  Serial.println(refreshPeriodInMs);
  if (currentMillis - startMillis >= refreshPeriodInMs) {
    refreshValue();
    /*
    valueRefreshed = true;
    refreshValueDurationInMs = millis() - currentMillis;
    Serial.println("refreshValue() " + label + "(time=" + String(refreshValueDurationInMs) + " ms)");
    */
    startMillis = currentMillis;
  }

  if (isValueChanged()) {
    draw();
  }

/*
  if (valueRefreshed) {
    screen->drawString(
      "       ",
      x + BUTTON_SIZE / 2,
      y + BUTTON_MARGIN + LED_MARGIN,
      1
    );
    screen->drawString(
      String(refreshValueDurationInMs),
      x + BUTTON_SIZE / 2,
      y + BUTTON_MARGIN + LED_MARGIN,
      1
    );
  }
*/

  touchStatus = isTouched();
  if (previousTouchStatus != touchStatus && touchStatus) {
    manageTouchDown();
  }
  /*if (touchStatus) {
    manageTouch();
    }*/
  if (previousTouchStatus != touchStatus && !touchStatus) {
    manageTouchUp();
  }
  previousTouchStatus = touchStatus;
  //Serial.println("Widget::refresh() " + label + "(time=" + String(millis() - currentMillis) + "ms )");
}

boolean Widget::isTouched() {
  return TouchScreen::isTouched
         && TouchScreen::x >= x
         && TouchScreen::x < x + BUTTON_SIZE
         && TouchScreen::y >= y
         && TouchScreen::y < y + BUTTON_SIZE;
}
