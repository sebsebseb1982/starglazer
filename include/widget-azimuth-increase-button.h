#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"

class WidgetAzimuthIncreaseButton : public Widget
{
private:
  ButtonStatus buttonStatus;
  ButtonStatus previousButtonStatus;

public:
  WidgetAzimuthIncreaseButton(
      unsigned int column,
      unsigned int row,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs);
  void draw();
  void refreshValue();
  boolean isValueChanged();
  void manageTouchUp();
  void manageTouchDown();
};
