#include <Arduino.h>
#include <TFT_eSPI.h>
#include "object-to-watch.h"
#include "widget.h"

class WidgetTrackedObjectButton : public Widget
{
private:
    boolean value;
    boolean previousValue;
    ButtonStatus buttonStatus;
    ButtonStatus previousButtonStatus;
    ObjectToWatch *trackedObject;

public:
    WidgetTrackedObjectButton(
        unsigned int column,
        unsigned int row,
        String label,
        TFT_eSPI *screen,
        ObjectToWatch *trackedObject,
        unsigned long refreshPeriodInMs);
    void draw();
    void refreshValue();
    boolean isValueChanged();
    void manageTouchUp();
    void manageTouchDown();
};
