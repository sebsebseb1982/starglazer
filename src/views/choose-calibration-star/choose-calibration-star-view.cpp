#include "choose-calibration-star-view.h"
#include "widget-choose-calibration-star-button.h"
#include "widget-gps-status.h"
#include "calibration-data.h"
#include "calibration-view.h"
#include "current-view-service.h"
#include "colors.h"
#include "duration.h"
#include "screen.h"
#include "debug.h"

// Hardcoded list of bright reference stars — codes follow the Simbad / Starglaze API convention.
// Each entry: { type, code, label }
static const ObjectToWatch CALIBRATION_STARS[] = {
    ObjectToWatch("deep-space-objects", "* alf UMi",  "Polaris"),
    ObjectToWatch("deep-space-objects", "* alf Lyr",  "Vega"),
    ObjectToWatch("deep-space-objects", "* alf CMa",  "Sirius"),
    ObjectToWatch("deep-space-objects", "* alf Boo",  "Arcturus"),
    ObjectToWatch("deep-space-objects", "* alf Aur",  "Capella"),
    ObjectToWatch("deep-space-objects", "* bet Ori",  "Rigel"),
    ObjectToWatch("deep-space-objects", "* alf Ori",  "Betelgeuse"),
    ObjectToWatch("deep-space-objects", "* alf Aql",  "Altair"),
    ObjectToWatch("deep-space-objects", "* alf Cyg",  "Deneb"),
    ObjectToWatch("deep-space-objects", "* alf Tau",  "Aldebaran"),
    ObjectToWatch("deep-space-objects", "* alf Vir",  "Spica"),
    ObjectToWatch("deep-space-objects", "* alf Leo",  "Regulus"),
};
static const int CALIBRATION_STARS_COUNT = sizeof(CALIBRATION_STARS) / sizeof(CALIBRATION_STARS[0]);

ObjectToWatch ChooseCalibrationStarView::selectedStar;

ChooseCalibrationStarView::ChooseCalibrationStarView(
    TFT_eSPI *screen,
    int starIndex,
    ObjectToWatch *returnToObject) : starIndex(starIndex),
                                     returnToObject(returnToObject),
                                     widgetsInitialized(false)
{
    this->screen = screen;
    selectedStar = ObjectToWatch("", "", "");
}

void ChooseCalibrationStarView::setup()
{
    if (starIndex == 0)
        CalibrationData::reset();

    screen->fillScreen(BACKGROUND_COLOR);
    screen->setTextColor(TFT_WHITE, BACKGROUND_COLOR);
    screen->setTextFont(2);

    // Progress label at the bottom of the grid (row 3)
    String progress = "Etoile de calibration (";
    progress += (starIndex + 1);
    progress += "/3)";
    screen->setCursor(10, 3 * BUTTON_SIZE + 8);
    screen->print(progress);

    buildWidgets();
}

void ChooseCalibrationStarView::buildWidgets()
{
    for (Widget *w : widgets)
        delete w;
    widgets.clear();

    for (int i = 0; i < CALIBRATION_STARS_COUNT; i++)
    {
        bool unavailable = CalibrationData::isStarAlreadySelected(CALIBRATION_STARS[i]);
        widgets.push_back(
            new WidgetChooseCalibrationStarButton(
                i % 6,
                i / 6,
                CALIBRATION_STARS[i],
                starIndex,
                unavailable,
                screen,
                FIVE_SECONDS));
    }

    widgets.push_back(
        new WidgetGPSStatus(
            5,
            3,
            "GPS",
            screen,
            FIVE_SECONDS));

    screen->fillRect(0, 0, SCREEN_WIDTH, 2 * BUTTON_SIZE, BACKGROUND_COLOR);
    for (Widget *widget : widgets)
    {
        widget->init();
    }
    widgetsInitialized = true;
}

void ChooseCalibrationStarView::loop()
{
    if (selectedStar.code == "")
    {
        for (Widget *widget : widgets)
        {
            widget->refresh();
        }
        return;
    }

    // A star was selected; store it and transition to CalibrationView
    CalibrationPoint point;
    point.star = selectedStar;
    point.apiAlt = 0;
    point.apiAz = 0;
    point.correctionAlt = 0;
    point.correctionAz = 0;
    CalibrationData::recordPoint(starIndex, point);

    ObjectToWatch *ret = returnToObject;
    returnToObject = nullptr; // prevent destructor from deleting it
    CurrentViewService::changeCurrentView(
        new CalibrationView(screen, starIndex, ret));
}

ChooseCalibrationStarView::~ChooseCalibrationStarView()
{
    for (Widget *widget : widgets)
    {
        delete widget;
    }
    if (returnToObject != nullptr)
    {
        delete returnToObject;
        returnToObject = nullptr;
    }
}
