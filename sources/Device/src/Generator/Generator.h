// 2022/11/01 11:12:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


namespace Generator
{
    void Enable();

    void Disable();

    bool IsEanbled();

    void SetFrequency(float);

    void SetDC(float);

    void SetAC(float);

    ValueADC ReadValue();
}
