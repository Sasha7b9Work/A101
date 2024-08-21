// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    // Функция возвращает частоту дискретизации для следующего сбора данных
    SampleRate AppendData();

    void Reset();

    REAL GetAbsAC(bool *correct);
    REAL GetAbsDC(bool *correct);
    REAL GetValueMin(bool *correct);
    REAL GetValueMax(bool *correct);
}
