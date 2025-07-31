#include "choosing-object-view.h"
#include "colors.h"
#include "starglaze-api.h"

ChoosingObjectView::ChoosingObjectView(
    TFT_eSPI *screen)
{
    this->screen = screen;
}

void ChoosingObjectView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
        StarGlazeAPI *starGlazeAPI = new StarGlazeAPI();
    starGlazeAPI->getCatalog();
}

void ChoosingObjectView::loop()
{

}