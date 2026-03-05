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
  bool coordinatesRequestSent;
  bool polarisCoordsReceived;
  ObjectToWatch *returnToObject;

public:
  static boolean calibrationDone;
  CalibrationView(TFT_eSPI *screen);
  CalibrationView(TFT_eSPI *screen, ObjectToWatch *returnToObject);
  ~CalibrationView();
  void setup();
  void loop();
};

#endif
