// 2022/11/01 13:59:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math.h"
#include <cstdlib>


double Math::Random(double min, double max)
{
    double distance = max - min;

    double scale = distance / (double)RAND_MAX;

    return min + (double)std::rand() * scale;
}
