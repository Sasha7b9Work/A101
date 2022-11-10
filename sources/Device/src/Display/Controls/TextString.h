// 2022/11/10 19:59:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


struct TextString
{
    static const int MAX_LEN = 32;

    TextString(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_colorText, const Color &_colorBack = Color::Count);
    void Disable();
    void Enable();
    void SetText(const char[MAX_LEN]);
private:
    char  text[MAX_LEN];
    int   x;
    int   y;
    int   width;
    int   height;
    int   font;
    Color colorText;
    Color colorBack;
};
