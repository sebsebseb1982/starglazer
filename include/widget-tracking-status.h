#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "equatorial-coordinates-service.h"
#include "tracked-object.h"

class WidgetTrackingStatus : public Widget
{
private:
  EquatorialCoordinates currentEquatorialCoordinates;
  EquatorialCoordinates previousEquatorialCoordinates;
  EquatorialCoordinatesService *equatorialCoordinatesService;
  TrackedObject *trackedObject;

public:
  WidgetTrackingStatus(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      TrackedObject *trackedObject,
      EquatorialCoordinatesService *equatorialCoordinatesService,
      unsigned long refreshPeriodInMs);
  void draw() override;
  void refreshValue() override;
  boolean isValueChanged() override;
  void manageTouch();
  void manageTouchUp();
  void manageTouchDown();
};
