#ifndef CALIBRATION_VIEW_H
#define CALIBRATION_VIEW_H
#include <list>
#include "view.h"
#include "widget.h"
#include "equatorial-coordinates-service.h"
#include "object-to-watch.h"

enum CalibrationState
{
    CALIBRATION_STATE_RESTORING_POSITION,
    CALIBRATION_STATE_CALIBRATING,
    CALIBRATION_STATE_WAITING_COORDS,
    CALIBRATION_STATE_HOMING,
    CALIBRATION_STATE_DONE
};

class CalibrationView : public View
{
private:
  TFT_eSPI *screen;
  std::list<Widget *> widgets;
  CalibrationState state;
  int starIndex;
  ObjectToWatch *returnToObject;
  bool coordinatesRequestSent;
  bool coordsReceived;
  float starApiAlt;
  float starApiAz;
  float angleBeforeAlt;
  float angleBeforeAz;

  void drawHeader();

public:
  static boolean calibrationDone;
  // starIndex: 0 = first star (sets home reference), 1 or 2 = subsequent stars (measure correction)
  // returnToObject: non-null when recalibrating from TrackingObjectView
  CalibrationView(TFT_eSPI *screen, int starIndex, ObjectToWatch *returnToObject);
  ~CalibrationView();
  void setup();
  void loop();
};

#endif
