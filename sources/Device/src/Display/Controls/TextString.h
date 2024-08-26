// 2022/11/10 19:59:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Nextion/Colors.h"

/*
*   Отрисовка прямо на поверхности страницы
*/

struct TextString
{
    static const int MAX_LEN = 32;

    TextString(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_colorText,
        bool _h_aligned = false, const Color &_colorBack = Color::Count);
    void Disable();
    void Enable();
    void SetText(const char[MAX_LEN]);
    void SetX(int _x) { x = _x; }
    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetWidth() const { return width; }
private:
    char  text[MAX_LEN];
    int   x;
    int   y;
    int   width;
    int   height;
    int   font;
    bool  h_aligned;
    Color colorText;
    Color colorBack;
};
