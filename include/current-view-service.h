#ifndef CURRENT_VIEW_SERVICE_H
#define CURRENT_VIEW_SERVICE_H
#include <TFT_eSPI.h>
#include <memory>
#include "view.h"

class CurrentViewService
{
private:
    static View* currentView;
    TFT_eSPI *screen;
public:
    CurrentViewService(TFT_eSPI *screen);
    void setup();
    void loop();
    static void changeCurrentView(View* newView);
};

#endif
