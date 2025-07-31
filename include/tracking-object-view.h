#ifndef TRACKING_OBJECT_VIEW_H
#define TRACKING_OBJECT_VIEW_H
#include "view.h"
#include "object-to-watch.h"
#include "widget.h"
#include "tracking-object-service.h"

#define widgetNumbers 7

class TrackingObjectView : public View
{
private:
  TFT_eSPI *screen;
  ObjectToWatch *trackedObject;
  Widget *widgets[widgetNumbers];

public:
  TrackingObjectView(
      TFT_eSPI *screen,
      ObjectToWatch *trackedObject);
  void setup();
  void loop();
};

#endif
