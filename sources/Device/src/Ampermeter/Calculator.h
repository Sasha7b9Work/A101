// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    void AppendData(const BufferADC &);

    void SetAverages(int num_ave);

    float GetAC();

    float GetDC();
}
