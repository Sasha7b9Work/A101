// 2022/10/19 08:58:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Painter
{
    namespace WaveInput
    {
        // 0 - маленький, 1 - большой
        void Disable(int size);
    }

    namespace WaveFFT
    {
        void Draw();

        // 0 - маленький, 1 - большой
        void Enable(int size);

        // 0 - маленький, 1 - большой
        void Disable(int size);
    }

    namespace Button
    {
        void SetText(pchar name_button, pchar text);

        void Highligth(pchar name_button, bool);

        void Eanble(pchar name_button);

        void Disable(pchar name_button);
    }
}
