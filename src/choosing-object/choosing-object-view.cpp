#include "choosing-object-view.h"
#include "colors.h"
#include "starglaze-api.h"
#include "duration.h"
#include "widget-back-button.h"
#include "widget-choose-object-button.h"
#include "current-view-service.h"
#include "tracking-object-view.h"

ObjectToWatch ChoosingObjectView::objectToWatch;

ChoosingObjectView::ChoosingObjectView(
    TFT_eSPI *screen)
{
    this->screen = screen;
    this->objectToWatch = ObjectToWatch("undefined","undefined","undefined");
    StarGlazeAPI *starGlazeAPI = new StarGlazeAPI();
    this->catalog = starGlazeAPI->getCatalog();
    delete starGlazeAPI;
}

void ChoosingObjectView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);

    this->widgets.push_back(
        new WidgetBackButton(
            0,
            0,
            "Back",
            this->screen,
            FIVE_SECONDS));

    int indexWidget = 1;

    for (Category category : catalog.categories)
    {
        for (ObjectToWatch objectToWatch : category.objectsToWatch)
        {
            this->widgets.push_back(
                new WidgetChooseObjectButton(
                    indexWidget,
                    0,
                    objectToWatch,
                    screen,
                    FIVE_SECONDS));

            indexWidget++;
        }
    }

    for (Widget *widget : widgets)
    {
        widget->init();
    }
}

void ChoosingObjectView::loop()
{
    if (this->objectToWatch.code == "undefined")
    {
        for (Widget *widget : widgets)
        {
            widget->refresh();
        }
    }
    else
    {
        CurrentViewService::changeCurrentView(
            new TrackingObjectView(
                this->screen,
                &this->objectToWatch));
    }
}

ChoosingObjectView::~ChoosingObjectView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
}