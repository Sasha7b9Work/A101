// 2022/11/10 22:25:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include <cstring>


void WindowMeasure::Clear()
{
    Nextion::Text::SetText(cntrlSign, "");
    Nextion::Text::SetText(cntrlDigits, "*.****");
    Nextion::Text::SetText(cntrlUnits, "");
}


void WindowMeasureAC::Clear()
{
    WindowMeasure::Clear();
    Nextion::Text::SetText(cntrlType, "AC:");
}


void WindowMeasureDC::Clear()
{
    WindowMeasure::Clear();
    Nextion::Text::SetText(cntrlType, "DC:");
}


void WindowMeasure::SetMeasure(pchar measure)
{
    if (measure[0])
    {
        if (cntrlSign[0])
        {
            char sign[2] = { measure[0], '\0' };
            Nextion::Text::SetText(cntrlSign, sign);
        }

        char buffer[7];

        std::memcpy(buffer, measure + 1, 6);
        buffer[6] = '\0';

        Nextion::Text::SetText(cntrlDigits, buffer);

        if (measure[std::strlen(measure) - 2] == ' ')
        {
            Nextion::Text::SetText(cntrlUnits, "A");
        }
        else
        {
            Nextion::Text::SetText(cntrlUnits, "mA");
        }
    }
}
