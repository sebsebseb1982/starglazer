#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "gps.h"

class WidgetGPSStatus : public Widget {
  private:
    GPSData currentGPSData;
    GPSData previousGPSData;
  public:
    WidgetGPSStatus(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs
    );
    void draw() override;
    void refreshValue() override;
    boolean isValueChanged() override;
    void manageTouchUp();
    void manageTouchDown();
};
