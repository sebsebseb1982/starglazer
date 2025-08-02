#ifndef CHOOSING_OBJECT_VIEW_H
#define CHOOSING_OBJECT_VIEW_H
#include <list>
#include "view.h"
#include "widget.h"
#include "object-to-watch.h"
#include "catalog.h"

class ChoosingObjectView : public View
{
private:
    TFT_eSPI *screen;
    std::list<Widget *> widgets;
    Catalog catalog;

public:
    static ObjectToWatch objectToWatch;
    ~ChoosingObjectView();
    ChoosingObjectView(
        TFT_eSPI *screen);
    void setup();
    void loop();
};

#endif
