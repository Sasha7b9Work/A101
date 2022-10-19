// 2022/10/19 08:58:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Primitives
{
    struct Rectangle
    {
        Rectangle(int w, int h) : width(w), height(h) {}
        void Draw(int x, int y, const Color & = Color::None);
    private:
        int width;
        int height;
    };
}
