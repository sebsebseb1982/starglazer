#include "choosing-object-view.h"
#include "network-queue.h"
#include "colors.h"
#include "duration.h"
#include "widget-choose-object-button.h"
#include "current-view-service.h"
#include "tracking-object-view.h"

ObjectToWatch ChoosingObjectView::objectToWatch;

ChoosingObjectView::ChoosingObjectView(
    TFT_eSPI *screen) : catalogLoaded(false), widgetsInitialized(false)
{
    this->screen = screen;
    this->objectToWatch = ObjectToWatch("undefined", "undefined", "undefined");
}

void ChoosingObjectView::setup()
{
    screen->fillScreen(BACKGROUND_COLOR);
    screen->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    screen->setTextFont(4);
    screen->setCursor(80, 140);
    screen->print("Chargement du catalogue...");
    // Trigger async catalog fetch
    NetworkQueue::sendCatalogRequest();
}

void ChoosingObjectView::buildWidgets()
{
    for (Widget *w : widgets)
        delete w;
    widgets.clear();

    int indexWidget = 0;
    for (Category category : catalog.categories)
    {
        for (ObjectToWatch objectToWatch : category.objectsToWatch)
        {
            widgets.push_back(
                new WidgetChooseObjectButton(
                    indexWidget % 6,
                    indexWidget / 6,
                    objectToWatch,
                    screen,
                    FIVE_SECONDS));
            indexWidget++;
        }
    }

    screen->fillScreen(BACKGROUND_COLOR);
    for (Widget *widget : widgets)
    {
        widget->init();
    }
    widgetsInitialized = true;
}

void ChoosingObjectView::loop()
{
    if (!catalogLoaded)
    {
        Catalog receivedCatalog;
        if (NetworkQueue::tryGetCatalog(receivedCatalog))
        {
            catalog = receivedCatalog;
            catalogLoaded = true;
            buildWidgets();
        }
        return;
    }

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