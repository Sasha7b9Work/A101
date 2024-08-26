// 2022/10/17 13:48:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Display
{
    void Init();

    void Update();

    namespace LabelStar
    {
        // Вызывается после поступившего измерения, чтобы зажечь звёздочку
        void Show();

        // Вызывается в главном цикле, чтобы потушить звёздочку, когда пройдёт достаточно времени
        void Update();
    }
}
