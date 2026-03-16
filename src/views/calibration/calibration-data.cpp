#include "calibration-data.h"
#include "debug.h"

CalibrationPoint CalibrationData::points[3];
int CalibrationData::completedStars = 0;
bool CalibrationData::calibrated = false;
bool CalibrationData::enabled = true;
float CalibrationData::coeffsAlt[3] = {0, 0, 0};
float CalibrationData::coeffsAz[3] = {0, 0, 0};

void CalibrationData::reset()
{
    completedStars = 0;
    calibrated = false;
    enabled = true;
    for (int i = 0; i < 3; i++)
    {
        points[i] = CalibrationPoint();
    }
    for (int i = 0; i < 3; i++)
    {
        coeffsAlt[i] = 0;
        coeffsAz[i] = 0;
    }
    DEBUG_PRINTLN("CalibrationData: reset");
}

void CalibrationData::recordPoint(int index, CalibrationPoint point)
{
    if (index < 0 || index > 2)
        return;
    points[index] = point;
    if (index + 1 > completedStars)
        completedStars = index + 1;

    String msg;
    msg += F("CalibrationData: recorded star ");
    msg += index;
    msg += F(" apiAlt=");
    msg += point.apiAlt;
    msg += F(" apiAz=");
    msg += point.apiAz;
    msg += F(" corrAlt=");
    msg += point.correctionAlt;
    msg += F(" corrAz=");
    msg += point.correctionAz;
    DEBUG_PRINTLN(msg);
}

// Solve: [alt0 az0 1] [a]   [corrAlt0]
//        [alt1 az1 1] [b] = [corrAlt1]
//        [alt2 az2 1] [c]   [corrAlt2]
// and the same system for azimuth coefficients.
// Uses Cramer's rule. Stores (0,0,0) for both if determinant is near zero.
void CalibrationData::computeCorrection()
{
    float a0 = points[0].apiAlt, b0 = points[0].apiAz;
    float a1 = points[1].apiAlt, b1 = points[1].apiAz;
    float a2 = points[2].apiAlt, b2 = points[2].apiAz;

    float det = a0 * (b1 - b2) - b0 * (a1 - a2) + (a1 * b2 - a2 * b1);

    if (fabsf(det) < 0.001f)
    {
        DEBUG_PRINTLN("CalibrationData: degenerate star configuration, correction disabled");
        for (int i = 0; i < 3; i++)
        {
            coeffsAlt[i] = 0;
            coeffsAz[i] = 0;
        }
        calibrated = true; // still mark calibrated, correction will be zero
        return;
    }

    float rA0 = points[0].correctionAlt, rA1 = points[1].correctionAlt, rA2 = points[2].correctionAlt;
    float rZ0 = points[0].correctionAz,  rZ1 = points[1].correctionAz,  rZ2 = points[2].correctionAz;

    // Cramer's rule for altitude correction coefficients
    float detA0 = rA0 * (b1 - b2) - b0 * (rA1 - rA2) + (rA1 * b2 - rA2 * b1);
    float detA1 = a0 * (rA1 - rA2) - rA0 * (a1 - a2) + (a1 * rA2 - a2 * rA1);
    float detA2 = a0 * (b1 * rA2 - b2 * rA1) - b0 * (a1 * rA2 - a2 * rA1) + rA0 * (a1 * b2 - a2 * b1);

    coeffsAlt[0] = detA0 / det;
    coeffsAlt[1] = detA1 / det;
    coeffsAlt[2] = detA2 / det;

    // Cramer's rule for azimuth correction coefficients
    float detZ0 = rZ0 * (b1 - b2) - b0 * (rZ1 - rZ2) + (rZ1 * b2 - rZ2 * b1);
    float detZ1 = a0 * (rZ1 - rZ2) - rZ0 * (a1 - a2) + (a1 * rZ2 - a2 * rZ1);
    float detZ2 = a0 * (b1 * rZ2 - b2 * rZ1) - b0 * (a1 * rZ2 - a2 * rZ1) + rZ0 * (a1 * b2 - a2 * b1);

    coeffsAz[0] = detZ0 / det;
    coeffsAz[1] = detZ1 / det;
    coeffsAz[2] = detZ2 / det;

    calibrated = true;

    DEBUG_PRINTLN("CalibrationData: correction computed");
    DEBUG_PRINT("  alt: a="); DEBUG_PRINT(coeffsAlt[0]);
    DEBUG_PRINT(" b="); DEBUG_PRINT(coeffsAlt[1]);
    DEBUG_PRINT(" c="); DEBUG_PRINTLN(coeffsAlt[2]);
    DEBUG_PRINT("  az:  d="); DEBUG_PRINT(coeffsAz[0]);
    DEBUG_PRINT(" e="); DEBUG_PRINT(coeffsAz[1]);
    DEBUG_PRINT(" f="); DEBUG_PRINTLN(coeffsAz[2]);
}

void CalibrationData::applyCorrection(float apiAlt, float apiAz,
                                      float &outAlt, float &outAz)
{
    if (!calibrated || !enabled)
    {
        outAlt = apiAlt;
        outAz = apiAz;
        return;
    }
    outAlt = apiAlt + (coeffsAlt[0] * apiAlt + coeffsAlt[1] * apiAz + coeffsAlt[2]);
    outAz  = apiAz  + (coeffsAz[0]  * apiAlt + coeffsAz[1]  * apiAz + coeffsAz[2]);
}

bool CalibrationData::isStarAlreadySelected(const ObjectToWatch &star)
{
    for (int i = 0; i < completedStars; i++)
    {
        if (points[i].star.code == star.code)
            return true;
    }
    return false;
}
