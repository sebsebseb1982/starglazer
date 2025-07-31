#ifndef WIDGET_H
#define WIDGET_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "gui.h"

class Widget
{
private:
  boolean touchStatus;
  boolean previousTouchStatus;
  unsigned long refreshPeriodInMs;
  unsigned long startMillis;
  unsigned long currentMillis;

public:
  TFT_eSPI *screen;
  unsigned int x;
  unsigned int y;
  unsigned int columnSpan;
  unsigned int rowSpan;

  String label;
  GUI gui;
  Widget(
      unsigned int columnPosition,
      unsigned int rowPosition,
      unsigned int columnSpan,
      unsigned int rowSpan,
      String label,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs);
  void init();
  void refresh();
  boolean isTouched();
  virtual void draw() = 0;
  virtual void refreshValue() = 0;
  virtual boolean isValueChanged() = 0;
  virtual void manageTouch() {};
  virtual void manageTouchUp() {};
  virtual void manageTouchDown() {};
};

#endif
