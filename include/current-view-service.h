#ifndef CURRENT_VIEW_SERVICE_H
#define CURRENT_VIEW_SERVICE_H
#include <TFT_eSPI.h>
#include <memory>
#include "view.h"

class CurrentViewService
{
private:
    View* currentView;
    TFT_eSPI *screen;
public:
    CurrentViewService(TFT_eSPI *screen);
    void setup();
    void loop();
};

#endif
