// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"


WindowMeasure::WindowMeasure(int x, int y, int font, pchar _title) :
    title(x, y, 175, 95, font, _title, Color::MeasureAC, Color::ButtonPress)
{

}


void WindowMeasure::Enable()
{
    title.Enable();
}


void WindowMeasure::Disable()
{
    title.Disable();
}


void WindowMeasure::SetText(char[TextString::MAX_LEN])
{

}
