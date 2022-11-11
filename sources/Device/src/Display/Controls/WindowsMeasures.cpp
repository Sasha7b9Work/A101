// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"


WindowMeasure::WindowMeasure(int x, int y, int font, pchar _title, const Color &back) :
    title(x, y, 172, HEIGHT, font, _title, Color::MeasureAC, false, back),
    point(x + 200, y, 28, HEIGHT, font, ".", Color::MeasureAC, true, back),
    digit1(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, back),
    digit2(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, back),
    digit3(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, back),
    digit4(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, back),
    digit5(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, back),
    units(x + 500, y, 50, HEIGHT, font, "", Color::MeasureAC, true, back),
    colorBack(back)
{
    digits[0] = &digit1;
    digits[1] = &digit2;
    digits[2] = &digit3;
    digits[3] = &digit4;
    digits[4] = &digit5;

    OnChangeRangeEvent();
}


void WindowMeasure::OnChangeRangeEvent()
{
    int range = InputRelays::GetRange();

    int x0 = title.GetX() + 250;

    Disable();

    static const int firsts[6] = { 0, 1, 2, 0, 1, 1 };
    static const int seconds[6] = { 1, 2, 3, 1, 2, 2 };

    for (int i = 0; i <= firsts[range]; i++)
    {
        digits[i]->SetX(x0);

        x0 += DELTA + WIDTH_DIGIT;
    }

    point.SetX(x0);

    x0 += DELTA + point.GetWidth();

    for (int i = seconds[range]; i < 5; i++)
    {
        digits[i]->SetX(x0);
        x0 += DELTA + WIDTH_DIGIT;
    }

    units.SetText(range < 3 ? "mA" : "A");

    Enable();
}


void WindowMeasure::Enable()
{
    title.Enable();
    point.Enable();
    units.Enable();

    for (int i = 0; i < 5; i++)
    {
        digits[i]->Enable();
    }
}


void WindowMeasure::Disable()
{
    Nextion::FillRect(title.GetX(), title.GetY(), WIDTH, HEIGHT, Color::Black);
}


void WindowMeasure::SetMeasure(char measure[TextString::MAX_LEN])
{
    int index = 0;

    for (int i = 0; (i < 7) && (index < 5); i++)
    {
        if (measure[i] >= '0' && measure[i] <= '9')
        {
            char str[2] = { '\0', '\0' };
            str[0] = measure[i];
            digits[index]->SetText(str);
            index++;
        }
    }
}
