#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "object-to-watch.h"

class WidgetChooseObjectButton : public Widget
{
private:
  boolean value;
  boolean previousValue;
  ButtonStatus buttonStatus;
  ButtonStatus previousButtonStatus;
  ObjectToWatch objectToWatch;

public:
  WidgetChooseObjectButton(
      unsigned int column,
      unsigned int row,
      ObjectToWatch objectToWatch,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs);
  void draw();
  void refreshValue();
  boolean isValueChanged();
  void manageTouchUp();
  void manageTouchDown();
};
