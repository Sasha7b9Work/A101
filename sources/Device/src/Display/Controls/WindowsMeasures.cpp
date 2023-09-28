// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Ampermeter.h"
#include "Display/Indicator.h"
#include <cstring>
#include <cstdio>


void WindowMeasure::Clear()
{
    cntrlSign.SetLabel("");
    cntrlDigits.SetLabel("*.****");
    cntrlUnits.SetLabel("");

    if (label_type[0] != '\0')
    {
        cntrlType.SetLabel(label_type);
    }

    buf_measure[0] = '\0';
}


void WindowMeasure::SetMeasure(float value)
{
    if (Page::Current() != PageMain::self)
    {
        return;
    }

    char buffer[30];

    buffer[0] = (value >= 0.0f) ? '+' : '-';

    std::sprintf(buffer + 1, "%5f", (double)value);

    SetMeasure(buffer);
}


void WindowMeasure::SetMeasure(pchar measure)
{
    if (measure[0])
    {
        if (cntrlSign.IsExist())
        {
            char sign[2] = { measure[0], '\0' };
            cntrlSign.SetLabel(sign);
        }

        char buffer[7];

        std::memcpy(buffer, measure + 1, 6);
        buffer[6] = '\0';

        cntrlDigits.SetLabel(buffer);

        if (measure[std::strlen(measure) - 2] == ' ')
        {
            cntrlUnits.SetLabel("A");
        }
        else
        {
            cntrlUnits.SetLabel("mA");
        }
    }
}


void WindowMeasure::Draw()
{
    int range = Range::Current();

    static const int after[6] = { 4, 3, 2, 4, 3, 3 };
    const pchar suffix = (range < 3) ? "mA" : "A";

    float value = 0.0f;
    bool out_of_range = true;

    if (type == TypeMeasure::DC)
    {
        value = Ampermeter::GetDC(&out_of_range);
    }
    else if (type == TypeMeasure::AC)
    {
        value = Ampermeter::GetAC(&out_of_range);
    }

    Indicator::ConvertDoubleToText(value, buf_measure, after[range], suffix);

    if (out_of_range)
    {
        for (int i = 0; (i < TextString::MAX_LEN) && (buf_measure[i] != '\0'); i++)
        {
            if (buf_measure[i] != '.') { buf_measure[i] = '^'; }
        }
    }
    else
    {
        SetMeasure(buf_measure);
    }
}
