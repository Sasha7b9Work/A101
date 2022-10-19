// 2022/10/19 08:58:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Primitives
{
    struct Rectangle
    {
        Rectangle(int w, int h) : width(w), height(h) {}
        void Draw(int x, int y, const Color & = Color::Count);
        void Fill(int x, int y, const Color & = Color::Count);
    private:
        int width;
        int height;
    };

    struct Line
    {
        void Draw(int x1, int y1, int x2, int y2, const Color & = Color::Count);
        void DrawV(int x, int y1, int y2, const Color & = Color::Count);
        void DrawH(int y, int x1, int x2, const Color & = Color::Count);
    };
}
