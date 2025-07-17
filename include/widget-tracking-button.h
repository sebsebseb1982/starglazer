#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "tracking-object-service.h"

class WidgetTrackingButton : public Widget {
  private:
    boolean value;
    boolean previousValue;
    ButtonStatus buttonStatus;
    ButtonStatus previousButtonStatus;

  public:
    WidgetTrackingButton(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      unsigned long refreshPeriodInMs
    );
    void draw();
    void refreshValue();
    boolean isValueChanged();
    void manageTouch();
    void manageTouchUp();
    void manageTouchDown();
};
