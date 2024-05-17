// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    // Функция возвращает частоту дискретизации для следующего сбора данных
    SampleRate AppendData();

    void Reset();

    // Значения, приведённые к пределу - mA для 2,20,200мА, амперы для 2A, 20A, 50A
    REAL GetRelativeAC(bool *correct);
    REAL GetRelativeDC(bool *correct);

    // Пересчитать значение таким образом, чтобы оCно нормально отображалось на дисплее
    REAL CastRealToDisplay(REAL, int range);
}
