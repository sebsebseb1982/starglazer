#ifndef TRACKING_OBJECT_VIEW_H
#define TRACKING_OBJECT_VIEW_H
#include "view.h"
#include "tracked-object.h"
#include "widget.h"

#define widgetNumbers 2

class TrackingObjectView : public View {
  private:
    TFT_eSPI *screen;
    TrackedObject trackedObject;
    Widget *widgets[widgetNumbers];
  public:
    TrackingObjectView(TFT_eSPI *screen, TrackedObject trackedObject);
    void setup();
    void loop();
};

#endif
