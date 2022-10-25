// 2022/10/17 13:48:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Measurer/BufferADC.h"


namespace Display
{
    void SetAC(float);

    void SetDC(float);

    void SetData(const BufferADC &);

    void Update();
}
