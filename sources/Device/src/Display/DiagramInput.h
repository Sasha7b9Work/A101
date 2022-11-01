// 2022/10/19 09:03:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace DiagramInput
{
    void Enable(bool);

    bool IsEnabled();

    void SetData(const BufferADC &);

    void Draw();

    // ”казывает немедленно перерисовать картикну
    void Repaint();
}
