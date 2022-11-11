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
    colorBack(back)
{
    digits[0] = &digit1;
    digits[1] = &digit2;
    digits[2] = &digit3;
    digits[3] = &digit4;
    digits[4] = &digit5;

//    int x0 = title.GetX() + 250;
//
//    for (int i = 0; i < 5; i++)
//    {
//        digits[i]->SetX(x0);
//
//        x0 += DELTA + WIDTH_DIGIT;
//    }

    OnChangeRangeEvent();
}


void WindowMeasure::OnChangeRangeEvent()
{
    int range = InputRelays::GetRange();

    int x0 = title.GetX() + 250;

    Disable();

    if (range == 0)
    {
        digits[0]->SetX(x0);

        x0 += DELTA + WIDTH_DIGIT;

        point.SetX(x0);

        x0 += DELTA + point.GetWidth();

        for (int i = 1; i < 5; i++)
        {
            digits[i]->SetX(x0);

            x0 += DELTA + WIDTH_DIGIT;
        }
    }
    else if (range == 1)
    {
        digits[0]->SetX(x0);

        x0 += DELTA + WIDTH_DIGIT;

        digits[1]->SetX(x0);

        x0 += DELTA + WIDTH_DIGIT;

        point.SetX(x0);

        x0 += DELTA + point.GetWidth();

        for (int i = 2; i < 5; i++)
        {
            digits[i]->SetX(x0);

            x0 += DELTA + WIDTH_DIGIT;
        }
    }

    Enable();
}


void WindowMeasure::Enable()
{
    title.Enable();
    point.Enable();

    for (int i = 0; i < 5; i++)
    {
        digits[i]->Enable();
    }
}


void WindowMeasure::Disable()
{
    Nextion::FillRect(title.GetX(), title.GetY(), WIDTH, HEIGHT, Color::Black);
}


void WindowMeasure::SetMeasure(char[TextString::MAX_LEN])
{

}
