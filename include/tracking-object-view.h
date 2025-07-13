#ifndef TRACKING_OBJECT_VIEW_H
#define TRACKING_OBJECT_VIEW_H
#include "view.h"
#include "tracked-object.h"
#include "widget.h"
#include "equatorial-coordinates-service.h"

#define widgetNumbers 3

class TrackingObjectView : public View {
  private:
    TFT_eSPI *screen;
    TrackedObject trackedObject;
    Widget *widgets[widgetNumbers];
    EquatorialCoordinatesService equatorialCoordinatesService;
    
  public:
    TrackingObjectView(TFT_eSPI *screen, TrackedObject trackedObject);
    void setup();
    void loop();
};

#endif
