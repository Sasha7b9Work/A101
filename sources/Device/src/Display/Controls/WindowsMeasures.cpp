// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"


WindowMeasure::WindowMeasure(int x, int y, int font, pchar _title) :
    title(x, y, 172, HEIGHT, font, _title, Color::MeasureAC, false, Color::ButtonPress),
    point(x + 200, y, 28, HEIGHT, font, ".", Color::MeasureAC, true, Color::ButtonPress),
    digit1(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, Color::ButtonPress),
    digit2(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, Color::ButtonPress),
    digit3(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, Color::ButtonPress),
    digit4(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, Color::ButtonPress),
    digit5(x + 200, y, WIDTH_DIGIT, HEIGHT, font, "8", Color::MeasureAC, true, Color::ButtonPress)
{
    digits[0] = &digit1;
    digits[1] = &digit2;
    digits[2] = &digit3;
    digits[3] = &digit4;
    digits[4] = &digit5;

    int x0 = x + 250;

    for (int i = 0; i < 5; i++)
    {
        digits[i]->SetX(x0);

        x0 += DELTA + WIDTH_DIGIT;
    }
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
    title.Disable();
    point.Disable();

    for (int i = 0; i < 5; i++)
    {
        digits[i]->Disable();
    }
}


void WindowMeasure::SetMeasure(char[TextString::MAX_LEN])
{

}
