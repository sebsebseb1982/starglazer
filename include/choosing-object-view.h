#ifndef CHOOSING_OBJECT_VIEW_H
#define CHOOSING_OBJECT_VIEW_H
#include "view.h"
#include "widget.h"

class ChoosingObjectView : public View
{
private:
    TFT_eSPI *screen;

public:
    ChoosingObjectView(
        TFT_eSPI *screen);
    void setup();
    void loop();
};

#endif
