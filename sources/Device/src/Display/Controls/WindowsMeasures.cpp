// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"


WindowMeasure::WindowMeasure(int x, int y, int font, pchar _title) :
    title(x, y, 172, 92, font, _title, Color::MeasureAC, false, Color::ButtonPress),
    point(x + 200, y, 28, 92, font, ".", Color::MeasureAC, true, Color::ButtonPress)
{

}


void WindowMeasure::Enable()
{
    title.Enable();
    point.Enable();
}


void WindowMeasure::Disable()
{
    title.Disable();
    point.Disable();
}


void WindowMeasure::SetMeasure(char[TextString::MAX_LEN])
{

}
