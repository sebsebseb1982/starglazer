#ifndef CALIBRATION_VIEW_H
#define CALIBRATION_VIEW_H
#include <list>
#include "view.h"
#include "widget.h"


class CalibrationView : public View
{
private:
  TFT_eSPI *screen;
  std::list<Widget *> widgets;

public:
  static boolean calibrationDone;
  CalibrationView(      TFT_eSPI *screen);
  ~CalibrationView();
  void setup();
  void loop();
};

#endif
