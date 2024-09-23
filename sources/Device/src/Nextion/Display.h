// 2022/10/17 13:48:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Display
{
    static const int WIDTH = 800;
    static const int HEIGHT = 480;

    void Init();

    void Update();

    void Clear();

    void SetBrightness();
}
