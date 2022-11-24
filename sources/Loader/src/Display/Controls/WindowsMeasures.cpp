// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"


WindowMeasure::WindowMeasure(int x, int y, bool _is_signed, int font, pchar _title, const Color &back) :
    title(x, y, 172, HEIGHT, font, _title, Color::MeasureAC, false, back),
    point(x + 200, y, 28, HEIGHT, font, ".", Color::MeasureAC, true, back),
    digit1(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "*", Color::MeasureAC, true),
    digit2(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "*", Color::MeasureAC, true),
    digit3(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "*", Color::MeasureAC, true),
    digit4(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "*", Color::MeasureAC, true),
    digit5(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "*", Color::MeasureAC, true),
    sign(x + 200, y,
#ifdef WIN32
        30,
#else
        60,
#endif
        HEIGHT, font, "+", Color::MeasureAC, true),
    units(x + 500, y,
#ifdef WIN32
        50,
#else
        150,
#endif
        HEIGHT, font, "", Color::MeasureAC, false),
    colorBack(back), is_signed(_is_signed)
{
    digits[0] = &digit1;
    digits[1] = &digit2;
    digits[2] = &digit3;
    digits[3] = &digit4;
    digits[4] = &digit5;
}


void WindowMeasure::Init()
{
    OnEventChangeRange();
}


void WindowMeasure::OnEventChangeRange()
{
    int range = Range::Current();

    int x0 = sign.GetX() + sign.GetWidth();

    static const int firsts[6] = { 0, 1, 2, 0, 1, 1 };
    static const int seconds[6] = { 1, 2, 3, 1, 2, 2 };

    for (int i = 0; i <= firsts[range]; i++)
    {
        digits[i]->SetX(x0);

        x0 += WIDTH_DIGIT;
    }

    point.SetX(x0);

    x0 += point.GetWidth();

    for (int i = seconds[range]; i < 5; i++)
    {
        digits[i]->SetX(x0);
        x0 += WIDTH_DIGIT;
    }

    units.SetX(digit5.GetX() + digit5.GetWidth() + 10);

    Enable();

    units.SetText(range < 3 ? "mA" : "A");

    for (int i = 0; i < 5; i++)
    {
        digits[i]->SetText("*");
    }
}


void WindowMeasure::Enable()
{
    title.Enable();
    point.Enable();
    units.Enable();
    if (is_signed)
    {
        sign.Enable();
    }

    for (int i = 0; i < 5; i++)
    {
        digits[i]->Enable();
    }
}


void WindowMeasure::Disable()
{
    Nextion::FillRect(title.GetX(), title.GetY(), WIDTH, HEIGHT,
        (colorBack.value == Color::Count.value) ? Color::Background : colorBack);
}


void WindowMeasure::SetMeasure(const char measure[TextString::MAX_LEN])
{
    int num_digits = 0;
    int num_zeros = 0;
    char symbol_sign = '\0';

    for (int i = 0; (i < 7) && (num_digits < 5); i++)
    {
        char symbol = measure[i];

        if (symbol == '-' || symbol == '+')
        {
            symbol_sign = symbol;
        }
        else if ((symbol >= '0' && symbol <= '9') || symbol == '*' || symbol == '^')
        {
            if (symbol == '0')
            {
                num_zeros++;
            }

            char str[2] = { symbol, '\0' };

            digits[num_digits]->SetText(str);
            num_digits++;

            if (symbol == '*' && is_signed)
            {
                sign.SetText("");
            }
        }
    }

    if(is_signed)
    {
        if (num_zeros != 5)
        {
            if (symbol_sign != '\0')
            {
                char str[2] = { symbol_sign, '\0' };
                sign.SetText(str);
            }
        }
        else
        {
            sign.SetText("");
        }
    }
}
