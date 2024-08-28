// 2022/11/10 19:59:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Controls/Label.h"
#include "Nextion/Nextion.h"
#include <cstring>


Label::Label(int _x, int _y, int _w, int _h, int _font, pchar _textRU, pchar _textEN, const Color &_colorText, bool _h_align, const Color &_colorBack) :
    x(_x), y(_y), width(_w), height(_h), font(_font), h_aligned(_h_align), colorText(_colorText), colorBack(_colorBack)
{
    std::strcpy(text[0], _textRU);
    std::strcpy(text[1], _textEN);
}


pchar Label::Text() const
{
    return text[set.lang];
}


void Label::Show()
{
    Nextion::DrawString(x, y, width, height, font, colorText,
        (colorBack.value == Color::Count.value) ? Color::Background : colorBack, Text(), h_aligned ? 1 : 0);
}


void Label::Hide()
{
    Nextion::DrawString(x, y, width, height, font, colorText, Color::Background, "");
}


void Label::SetVisible(bool visible)
{
    visible ? Show() : Hide();
}


void Label::SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN])
{
    if (std::strcmp(_textRU, text[Lang::RU]) != 0)
    {
        std::strcpy(text[Lang::RU], _textRU);
    }

    if (std::strcmp(_textEN, text[Lang::EN]) != 0)
    {
        std::strcpy(text[Lang::EN], _textEN);
    }

    Show();
}
