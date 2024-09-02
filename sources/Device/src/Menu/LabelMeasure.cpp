// 2024/08/30 16:19:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include <cstring>
#include <cmath>
#include <cstdio>


namespace LM
{
    static const Font::E fonts[SizeMeasure::Count] =
    {
        Font::_4,
        Font::_0
    };

    static const int16 widths[SizeMeasure::Count] =
    {
        518,
        240,
        391
    };

    static const int16 width_name[SizeMeasure::Count] =     // Ширина поля имени измерения (DC, Iampl, etc)
    {
        50,
        50,
        0
    };

    static const int16 height_name[SizeMeasure::Count] =
    {
        50,
        50,
        0
    };

    static const int16 heights[SizeMeasure::Count] =
    {
        85,
        40,
        85
    };
}


LabelMeasure::LabelMeasure(TypeMeasure::E _type, SizeMeasure::E _size, int _x, int _y, void (*_funcOnPress)()) :
    Label("", "", { _x, _y, LM::widths[_size], LM::heights[_size] }, LM::fonts[_size], _funcOnPress),
    name_measure("", "", { _x, _y, LM::width_name[_type], LM::height_name[_type] }, LM::fonts[_size]),
    type_measure(_type)
{
}


void LabelMeasure::Reset()
{

}


void LabelMeasure::SetMeasure(const Measure &measure, int range)
{
    if (!measure.correct)
    {
        Reset();
    }
    else
    {
        if (measure.out_of_range)
        {
            std::strcpy(buf_measure, "   OL");
        }
        else
        {
            static const int after[6] = { 4, 3, 2, 4, 3, 3 };
            const pchar suffix = (range < 3) ? "mA" : "A";

            ConvertRealToText(measure.value_abs / (Range::Current() > 2 ? 1e3 : 1.0), buf_measure, after[range], suffix);
        }

        SetMeasure(buf_measure);
    }
}


void LabelMeasure::Flash()
{

}


void LabelMeasure::Draw()
{
    Nextion::DrawRect(rect, Color::White);
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


void LabelMeasure::ConvertRealToText(REAL value, char out[Label::MAX_LEN], int after, pchar suffix)
{
    std::strcpy(out, value < 0.0 ? "-" : "+");

    value = std::fabs(value);

    int before = 5 - after;

    if (before == 3)
    {
        if (value < 10.0)
        {
            std::strcat(out, "00");
        }
        else if (value < 100.0)
        {
            std::strcat(out, "0");
        }
    }

    if (before == 2 && value < 10.0)
    {
        std::strcat(out, "0");
    }

    // Отбрасываем цифры, которых нет на экране - во избежание бага, когда 10 превращеется в 1 (при 9.999999)
    {
        for (int i = 0; i < after; i++)
        {
            value *= 10.0;
        }

        for (int i = 0; i < after; i++)
        {
            value /= 10.0;
        }
    }

    char buffer[Label::MAX_LEN];

    char format[] = { '%', '0', (char)((before + 1) | 0x30), '.', (char)(after | 0x30), 'f', ' ', '%', 's', '\0' };

    std::sprintf(buffer, format, (double)value, suffix);

    std::strcat(out, buffer);
}


bool TypeMeasure::ExistFieldSign() const
{
    static const bool exist[TypeMeasure::Count] =
    {
        true,
        false,
        false,
        false,
        true,
        true,
        false
    };

    return exist[value];
}


void LabelMeasure::DrawSign(pchar)
{

}


void LabelMeasure::DrawDigits(pchar)
{

}


void LabelMeasure::DrawUnits(pchar, pchar)
{

}


void LabelMeasure::SetMeasure(pchar measure)
{
    if (measure[0])
    {
        if (type_measure.ExistFieldSign())
        {
            char sign[2] = { measure[0], '\0' };
            DrawSign(sign);
        }

        char buffer[7];

        std::memcpy(buffer, measure + 1, 6);
        buffer[6] = '\0';

        DrawDigits(buffer);

        if (measure[std::strlen(measure) - 2] == ' ')
        {
            DrawUnits("А", "A");
        }
        else
        {
            DrawUnits("мА", "mA");
        }
    }
}
