// 2024/08/30 16:19:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"


namespace LM
{
    static const Font::E fonts[SizeMeasure::Count] =
    {
        Font::_4,
        Font::_0
    };

    static const int widths[SizeMeasure::Count] =
    {
        518,
        240
    };

    static const int heights[SizeMeasure::Count] =
    {
        85,
        40
    };
}


LabelMeasure::LabelMeasure(TypeMeasure::E, SizeMeasure::E _size, int _x, int _y, void (*_funcOnPress)()) :
    Label("", "", _x, _y, LM::widths[_size], LM::heights[_size], LM::fonts[_size], _funcOnPress)
{

}


void LabelMeasure::Reset()
{

}


void LabelMeasure::SetMeasure(const Measure &, int)
{

}


void LabelMeasure::Flash()
{

}


pchar LabelMeasure::GetUnits() const
{
    return "";
}


pchar LabelMeasure::GetDigits() const
{
    return "";
}


pchar LabelMeasure::GetSign() const
{
    return "";
}
