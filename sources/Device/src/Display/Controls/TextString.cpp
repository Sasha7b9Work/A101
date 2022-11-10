// 2022/11/10 19:59:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/TextString.h"
#include "Nextion/Nextion.h"
#include <cstring>


TextString::TextString(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_colorText, const Color &_colorBack) :
    x(_x), y(_y), width(_w), height(_h), font(_font), colorText(_colorText), colorBack(_colorBack)
{
    std::strcpy(text, _text);
}


void TextString::Enable()
{
    Nextion::DrawString(x, y, width, height, font, colorText, (colorBack.value == Color::Count.value) ? Color::Background : Color::ButtonPress, text);
}


void TextString::Disable()
{
    Nextion::DrawString(x, y, width, height, font, colorText, Color::Background, "");
}


void TextString::SetText(const char _text[MAX_LEN])
{
    if (std::strcmp(_text, text) == 0)
    {
        return;
    }

    std::strcpy(text, _text);

    Enable();
}
