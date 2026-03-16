#ifndef CALIBRATION_DATA_H
#define CALIBRATION_DATA_H

#include <Arduino.h>
#include "object-to-watch.h"

struct CalibrationPoint
{
    ObjectToWatch star;
    float apiAlt;
    float apiAz;
    float correctionAlt;
    float correctionAz;
};

class CalibrationData
{
public:
    static CalibrationPoint points[3];
    static int completedStars;
    static bool calibrated;
    static bool enabled;

    // Affine correction coefficients:
    // corrected_alt = api_alt + (coeffsAlt[0]*api_alt + coeffsAlt[1]*api_az + coeffsAlt[2])
    // corrected_az  = api_az  + (coeffsAz[0]*api_alt  + coeffsAz[1]*api_az  + coeffsAz[2])
    static float coeffsAlt[3];
    static float coeffsAz[3];

    static void reset();
    static void recordPoint(int index, CalibrationPoint point);

    // Solve 3x3 system via Cramer's rule.
    // Must be called after all 3 points are recorded.
    static void computeCorrection();

    // Apply affine correction to raw API coordinates.
    // Falls back to identity if not calibrated.
    static void applyCorrection(float apiAlt, float apiAz,
                                float &outAlt, float &outAz);

    static bool isStarAlreadySelected(const ObjectToWatch &star);
};

#endif
