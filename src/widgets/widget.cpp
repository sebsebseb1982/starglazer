#include "widget.h"
#include "screen.h"
#include "touch-screen.h"
#include "motor.h"

Widget::Widget(
    unsigned int columnPosition,
    unsigned int rowPosition,
    unsigned int columnSpan,
    unsigned int rowSpan,
    String label,
    TFT_eSPI *screen,
    unsigned long refreshPeriodInMs)
{
  this->x = columnPosition * BUTTON_SIZE;
  this->y = rowPosition * BUTTON_SIZE;
  this->columnSpan = columnSpan;
  this->rowSpan = rowSpan;
  this->label = label;
  this->screen = screen;
  this->refreshPeriodInMs = refreshPeriodInMs;
  this->gui = (screen);
  this->startMillis = millis();
}

void Widget::init()
{
  this->previousTouchStatus = false;
  refreshValue();
  draw();
}

void Widget::refresh()
{
  currentMillis = millis();
  boolean valueRefreshed = false;
  unsigned long refreshValueDurationInMs = 0;
  if (currentMillis - startMillis >= refreshPeriodInMs)
  {
    refreshValue();
    startMillis = currentMillis;
  }

  if (isValueChanged())
  {
    draw();
  }

  touchStatus = isTouched();
  if (previousTouchStatus != touchStatus && touchStatus)
  {
    String message;
    message += F("Button ");
    message += label;
    message += F(" pushed");
    Serial.println(message);
    manageTouchDown();
    digitalWrite(2, HIGH);
  }

  if (previousTouchStatus != touchStatus && !touchStatus)
  {
    String message;
    message += F("Button ");
    message += label;
    message += F(" released");
    Serial.println(message);
    manageTouchUp();
    digitalWrite(2, LOW);
  }
  previousTouchStatus = touchStatus;
}

boolean Widget::isTouched()
{
  boolean isTouched = 
  TouchScreen::isTouched
    && TouchScreen::x >= x
    && TouchScreen::x < x + columnSpan * BUTTON_SIZE 
    && TouchScreen::y >= y 
    && TouchScreen::y < y + rowSpan * BUTTON_SIZE;

  if (isTouched)
  {
    String message;
    message += F("Button ");
    message += label;
    message += F(" touched");
    Serial.println(message);
  }

  return isTouched;
}
