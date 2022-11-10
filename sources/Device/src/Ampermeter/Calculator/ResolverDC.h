// 2022/11/08 13:02:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace ResolverDC
{
    double Calculate(const BufferADC &, int period);
}
