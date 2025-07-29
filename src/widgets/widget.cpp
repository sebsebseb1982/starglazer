#include "widget.h"
#include "screen.h"
#include "touch-screen.h"
#include "motor.h"

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
  refreshValue();
  draw();
}

void Widget::refresh() {
  currentMillis = millis();
  boolean valueRefreshed = false;
  unsigned long refreshValueDurationInMs = 0;
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
    /*String message;
    message += F("Button ");
    message += label;
    message += F(" pushed");
    Serial.println(message);*/
    manageTouchDown();
    //Motor::turn();
  }
  /*if (touchStatus) {
    manageTouch();
    }*/
  if (previousTouchStatus != touchStatus && !touchStatus) {
    /*String message;
    message += F("Button ");
    message += label;
    message += F(" released");
    Serial.println(message);*/
    manageTouchUp();
  }
  previousTouchStatus = touchStatus;
  //Serial.println("Widget::refresh() " + label + "(time=" + String(millis() - currentMillis) + "ms )");
}

boolean Widget::isTouched() {
  Serial.print("Widget::isTouched() ");
  Serial.print(label);
  Serial.println(" Start");
boolean isTpivchref = TouchScreen::isTouched
         && TouchScreen::x >= x
         && TouchScreen::x < x + BUTTON_SIZE
         && TouchScreen::y >= y
         && TouchScreen::y < y + BUTTON_SIZE;
  Serial.print("Widget::isTouched() ");
  Serial.print(label);
  Serial.println(" Stop");
  return isTpivchref;
}
