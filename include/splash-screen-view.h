#ifndef SPLASH_SCREEN_VIEW_H
#define SPLASH_SCREEN_VIEW_H
#include "view.h"
#include <TFT_eSPI.h>

class SplashScreenView : public View
{
private:
    TFT_eSPI *screen;

public:
    SplashScreenView(TFT_eSPI *screen);
    void setup();
    void loop();
};

#endif
