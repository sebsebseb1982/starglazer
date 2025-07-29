#include <Arduino.h>
#include <TFT_eSPI.h>
#include "tracked-object.h"
#include "widget.h"

class WidgetTrackedObjectButton : public Widget
{
private:
    boolean value;
    boolean previousValue;
    ButtonStatus buttonStatus;
    ButtonStatus previousButtonStatus;
    TrackedObject *trackedObject;

public:
    WidgetTrackedObjectButton(
        unsigned int column,
        unsigned int row,
        String label,
        TFT_eSPI *screen,
        TrackedObject *trackedObject,
        unsigned long refreshPeriodInMs);
    void draw();
    void refreshValue();
    boolean isValueChanged();
    void manageTouch();
    void manageTouchUp();
    void manageTouchDown();
};
