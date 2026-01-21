#include "RailUtils.h"
#include "tipuriSine.h"
#include <utils/math_utils.h>

float DirToAngle(RailDir dir)
{
    switch (dir) {
    case RailDir::Forward:  return 0.0f;
    case RailDir::Right:    return RADIANS(90);
    case RailDir::Backward: return RADIANS(180);
    case RailDir::Left:     return RADIANS(-90);
    }
    return 0.0f;
}