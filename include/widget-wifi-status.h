#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"

class WidgetWifiStatus : public Widget
{
private:
  boolean currentWifiStatus;
  boolean previousWifiStatus;

public:
  WidgetWifiStatus(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs);
  void draw() override;
  void refreshValue() override;
  boolean isValueChanged() override;
  void manageTouch();
  void manageTouchUp();
  void manageTouchDown();
};
