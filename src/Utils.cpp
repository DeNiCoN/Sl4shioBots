#include "Utils.hpp"
#define _USE_MATH_DEFINES
#include <math.h>


namespace utils
{
    float clockwiseAngle(vec2 dir)
    {
        return M_PI + atan2f(dir.y, dir.x);
    }
}
