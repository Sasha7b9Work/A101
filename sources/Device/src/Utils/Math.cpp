// 2022/11/01 13:59:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"
#include <cstdlib>


float Math::Random(float min, float max)
{
    float distance = max - min;

    float scale = distance / (float)RAND_MAX;

    return min +  (float)std::rand() * scale;
}
