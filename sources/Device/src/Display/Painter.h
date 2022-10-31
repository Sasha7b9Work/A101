// 2022/10/19 08:58:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


struct Line
{
    void Draw(int x1, int y1, int x2, int y2, const Color & = Color::Count);
};


namespace Painter
{
    void DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar);

    namespace WaveInput
    {
        void Draw(uint8 *points, int num_points);

        // 0 - маленький, 1 - большой
        void Enable(int size);

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
