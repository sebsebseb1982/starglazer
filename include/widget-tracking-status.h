#include <Arduino.h>
#include <TFT_eSPI.h>

#include "widget.h"
#include "equatorial-coordinates-service.h"
#include "tracked-object.h"
#include "tracking-object-service.h"

class WidgetTrackingStatus : public Widget
{
private:
  EquatorialCoordinates previousEquatorialCoordinates;
  TrackedObject *trackedObject;
  void drawAzimuthStatus(unsigned int x, unsigned int y);
  void drawAzimuthCompass(unsigned int x, unsigned int y);

  void drawAltitudeStatus(unsigned int x, unsigned int y);
  void drawAltitudeChart(unsigned int x, unsigned int y);

  void drawDistanceStatus(unsigned int x, unsigned int y);
  void drawDistanceChart(unsigned int x, unsigned int y);

public:
  WidgetTrackingStatus(
      unsigned int column,
      unsigned int row,
      String label,
      TFT_eSPI *screen,
      TrackedObject *trackedObject,
      unsigned long refreshPeriodInMs);
  void draw() override;
  void refreshValue() override;
  boolean isValueChanged() override;
  void manageTouch();
  void manageTouchUp();
  void manageTouchDown();
};
