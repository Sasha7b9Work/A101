// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include <cstring>
#include <cstdio>


void WindowMeasure::Clear()
{
    cntrlSign.SetLabel("");
    cntrlDigits.SetLabel("*.****");
    cntrlUnits.SetLabel("");
}


void WindowMeasureAC::Clear()
{
    WindowMeasure::Clear();
    cntrlType.SetLabel("AC:");
}


void WindowMeasureDC::Clear()
{
    WindowMeasure::Clear();
    cntrlType.SetLabel("DC:");
}


void WindowMeasureMAX::Clear()
{
    WindowMeasure::Clear();
    cntrlType.SetLabel("Imax:");
}


void WindowMeasure::SetMeasure(float value)
{
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
