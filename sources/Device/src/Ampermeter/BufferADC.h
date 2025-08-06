// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


namespace BufferADC
{
    static const int SIZE = 16 * 1024;

    void Push(ValueADC word);

    int NumElements();

    bool IsFull();

    void Clear();

    void CalculateLimits();

    REAL MinReal();

    REAL MaxReal();

    void LogUART();

    ValueADC At(int i);

    // Прменить медианный фильтр по трём
    void MiddleOf3();

    // Сгладить сигнал
    void SmoothOut();
}
