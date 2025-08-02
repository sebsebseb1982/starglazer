#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "starglaze-api.h"

class WidgetAPIStatus : public Widget
{
private:
  boolean currentAPIStatus;
  boolean previousAPIStatus;
  StarGlazeAPI *starGlazeAPI;

public:
  ~WidgetAPIStatus();
  WidgetAPIStatus(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs);
  void draw() override;
  void refreshValue() override;
  boolean isValueChanged() override;
  void manageTouchUp();
  void manageTouchDown();
};
