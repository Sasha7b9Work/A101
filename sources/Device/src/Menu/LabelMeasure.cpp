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
        Font::_7_GOST_B_144b,
        Font::_0_GB34b,
        Font::_4_GB80b
    };

    static const int16 heights[SizeMeasure::Count] =
    {
        120,
        40,
        85
    };
}


LabelMeasure::LabelMeasure(TypeMeasure::E _type, SizeMeasure::E _size, int _x, int _y, void (*_funcOnPress)(Item *, bool)) :
    Label(true, "", "", { _x, _y, 0, LM::heights[_size] }, LM::fonts[_size], _funcOnPress),
    type_measure(_type)
{
    int x = 0;
    int width = GetWidth(_size, 0);
    int height = LM::heights[_size];

    label_name = Label(false, type_measure.Title(Lang::RU), type_measure.Title(Lang::EN),
        { _x + x, _y, width, height },
        LM::fonts[_size], EmptyFuncVIemB, Color::White, Color::Count, true, false);

    x += width;
    width = GetWidth(_size, 1);

    label_sign = Label(false, "", "",
        { _x + x, _y, width, height },
        LM::fonts[_size], EmptyFuncVIemB, Color::White, Color::Count, false, false);

    x += width;
    width = GetWidth(_size, 2);

    label_digits = Label(false, "", "",
        { _x + x, _y, width, height },
        LM::fonts[_size], EmptyFuncVIemB, Color::White, Color::Count, true, false);

    x += width;
    width = GetWidth(_size, 3);

    label_units = Label(false, "", "",
        { _x + x, _y, width, height },
        LM::fonts[_size], EmptyFuncVIemB, Color::White, Color::Count, false, false);

    rect.width = (int16)(GetWidth(_size, 0) + GetWidth(_size, 1) + GetWidth(_size, 2) + GetWidth(_size, 3));
}


void LabelMeasure::SetCoord(const Coord &coord)
{
    Coord delta = coord - Item::GetCoord();

    Item::SetCoord(coord);

    label_name.SetCoord(label_name.GetCoord() + delta);
    label_sign.SetCoord(label_sign.GetCoord() + delta);
    label_digits.SetCoord(label_digits.GetCoord() + delta);
    label_units.SetCoord(label_units.GetCoord() + delta);
}


Coord Item::GetCoord() const
{
    return { rect.x, rect.y };
}


void Item::SetCoord(const Coord &coord)
{
    rect.x = coord.x;
    rect.y = coord.y;
}


int LabelMeasure::GetWidth(SizeMeasure::E size, int label)
{
    static const int width[SizeMeasure::Count][4] =
    {
        {175, 64, 390, 170},
        {80,  17, 100, 55},
        {10, 40, 250, 100}
    };

    return width[size][label];
}


#ifdef DRAW_DEBUG_LINES

bool LabelMeasure::Draw()
{
    if (!IsEnabled())
    {
        return false;
    }

    if (need_draw)
    {
        if (IsShown())
        {
            Nextion::DrawRect(rect, Color::White);

            if (label_name.Draw())
            {
                Nextion::DrawRect(label_name.GetRect());
            }

            if (label_sign.Draw())
            {
                Nextion::DrawRect(label_sign.GetRect());
            }

            if (label_digits.Draw())
            {
                Nextion::DrawRect(label_digits.GetRect());
            }

            if (label_units.Draw())
            {
                Nextion::DrawRect(label_units.GetRect());
            }
        }
        else
        {
            Nextion::FillRect(rect, colorBack);
        }

        need_draw = false;

        return true;
    }

    return false;
}

#else

bool LabelMeasure::Draw()
{
    if (!IsEnabled())
    {
        return false;
    }

    if (need_draw)
    {
        if (IsShown())
        {
            label_name.Draw();
            label_sign.Draw();
            label_digits.Draw();
            label_units.Draw();
        }
        else
        {
            Nextion::FillRect(rect, colorBack);
        }

        need_draw = false;

        return true;
    }

    return false;
}

#endif


void LabelMeasure::SetShown(bool show)
{
    label_name.SetShown(show);
    label_sign.SetShown(show);
    label_digits.SetShown(show);
    label_units.SetShown(show);

    Item::SetShown(show);
}


void LabelMeasure::Refresh()
{
    label_name.Refresh();
    label_sign.Refresh();
    label_digits.Refresh();
    label_units.Refresh();
}


void LabelMeasure::Reset()
{
    int range = Range::Current();

    if (range == 0 || range == 3)
    {
        label_digits.SetText("*.****", "*.****");
    }
    else if (range == 1 || range == 4 || range == 5)
    {
        label_digits.SetText("**.***", "**.***");
    }
    else if (range == 2)
    {
        label_digits.SetText("***.**", "***.**");
    }

    if (type_measure.IsFrequency())
    {
        label_digits.SetText("*****", "*****");

        label_units.SetText("", "");
    }
    else
    {
        label_units.SetText(range < 3 ? "мA" : "A", range < 3 ? "мA" : "A");
    }

    label_sign.SetText("", "");

    buf_measure[0] = '\0';

    need_draw = true;
}


void LabelMeasure::SetMeasure(pchar message_ru, pchar message_en)
{
    label_units.SetText("", "");

    label_digits.SetText(message_ru, message_en);

    need_draw = true;
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
            std::strcpy(buf_measure, type_measure.IsFrequency() ? "*****" : " OL");
        }
        else
        {
            static const int after[6] = { 4, 3, 2, 4, 3, 3 };

            if (type_measure.IsFrequency())
            {
                frequency = measure.value_abs;

                ConvertRealToText(frequency, buf_measure, TypeMeasure::GetNumDigitsAfterComma(frequency), 3);
            }
            else
            {
                ConvertRealToText(measure.value_abs / (Range::Current() > 2 ? 1e3 : 1.0), buf_measure, after[range], 5);
            }
        }

        if (type_measure.IsFrequency())
        {
            char ru[32];
            char en[32];

            TypeMeasure::GetUnitsForFrequnesy(frequency, ru, en);

            SetMeasure(buf_measure, ru, en);
        }
        else
        {
            SetMeasure(buf_measure, (range < 3 ? "мА" : "А"), (range < 3 ? "mA" : "A"));
        }
    }

    need_draw = true;
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


void LabelMeasure::ConvertRealToText(REAL value, char out[Label::MAX_LEN], int after, int num_digits)
{
    if (type_measure.IsFrequency())
    {
        while (value >= 1e3)
        {
            value /= 1e3;
        }

        while (value < 1e0)
        {
            value *= 1e3;
        }
    }

    std::strcpy(out, value < 0.0 ? "-" : "+");

    value = std::fabs(value);

    int before = num_digits - after;

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

        value = (float)((int)value);

        for (int i = 0; i < after; i++)
        {
            value /= 10.0;
        }
    }

    char buffer[Label::MAX_LEN];

    char format[] = { '%', '0', (char)((before + 1) | 0x30), '.', (char)(after | 0x30), 'f', '\0', '\0', '\0'};

    if (type_measure.IsFrequency() && after == 0)
    {
        std::strcpy(format, "%03.0f.");
    }

    std::sprintf(buffer, format, (double)value);

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


pchar TypeMeasure::Title(Lang::E lang) const
{
    static const pchar titles[TypeMeasure::Count][Lang::Count] =
    {
        {"DC:",    "DC:"},
        {"AC:",    "AC:"},
        {"Iамп:",  "Iamp:"},
        {"Iпп:",   "Ipp:"},
        {"Iмин:",  "Imin:"},
        {"Iмакс:", "Imax:"},
        {"F:",     "F:"}
    };

    return titles[value][lang];
}


void LabelMeasure::DrawSign(pchar sign)
{
    label_sign.SetText(sign, sign);
}


void LabelMeasure::DrawDigits(pchar digits)
{
    label_digits.SetText(digits, digits);
}


void LabelMeasure::SetMeasure(pchar measure, pchar units_ru, pchar units_en)
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

        label_units.SetText(units_ru, units_en);
    }
}
