// 2022/10/19 08:58:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Painter
{
    void DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar);

    namespace WaveInput
    {
        void Draw(uint8 *points, int num_points);

        // 0 - ���������, 1 - �������
        void Enable(int size);

        // 0 - ���������, 1 - �������
        void Disable(int size);
    }

    namespace WaveFFT
    {
        void Draw();

        // 0 - ���������, 1 - �������
        void Enable(int size);

        // 0 - ���������, 1 - �������
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
