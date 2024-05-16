// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    // Функция возвращает частоту дискретизации для следующего сбора данных
    SampleRate AppendData();

    void Reset();

    REAL GetAC(bool *correct);

    REAL GetDC(bool *correct);

    // Пересчитать значение таким образом, чтобы оно нормально отображалось на дисплее
    REAL CastRealToDisplay(REAL, int range);
}
