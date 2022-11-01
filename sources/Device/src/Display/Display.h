// 2022/10/17 13:48:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Display
{
    void Init();

    void SetData(const BufferADC &);

    void Update();
}
