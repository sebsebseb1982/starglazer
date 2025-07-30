#include "choosing-object-view.h"
#include "colors.h"

ChoosingObjectView::ChoosingObjectView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void ChoosingObjectView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
}

void ChoosingObjectView::loop()
{
}