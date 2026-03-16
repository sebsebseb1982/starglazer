#ifndef CHOOSE_CALIBRATION_STAR_VIEW_H
#define CHOOSE_CALIBRATION_STAR_VIEW_H

#include <list>
#include "view.h"
#include "widget.h"
#include "object-to-watch.h"

class ChooseCalibrationStarView : public View
{
private:
    TFT_eSPI *screen;
    int starIndex;
    ObjectToWatch *returnToObject;
    std::list<Widget *> widgets;
    bool widgetsInitialized;

    void buildWidgets();

public:
    static ObjectToWatch selectedStar;

    ChooseCalibrationStarView(
        TFT_eSPI *screen,
        int starIndex,
        ObjectToWatch *returnToObject);
    ~ChooseCalibrationStarView();
    void setup();
    void loop();
};

#endif
