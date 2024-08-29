// 2022/11/10 19:59:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Nextion/Colors.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


/*
*   Отрисовка прямо на поверхности страницы
*/


struct Label
{
    static const int MAX_LEN = 32;

    Label(int _x, int _y, int _w, int _h, int _font, pchar _textRU, pchar _textEN, const Color &_colorText = Color::White,
        bool _h_aligned = false, const Color &_colorBack = Color::Count);
    void Hide();
    void Show();
    void SetVisible(bool);
    void SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN]);
    void SetX(int _x) { rect.x = (int16)_x; }
    int GetX() const { return rect.x; }
    int GetY() const { return rect.y; }
    int GetWidth() const { return rect.width; }
    pchar Text() const;
    pchar Text(Lang::E lang) const { return text[lang]; }
private:
    char  text[Lang::Count][MAX_LEN];
    Rect  rect;
    int   font;
    bool  h_aligned;
    Color colorText;
    Color colorBack;
};
