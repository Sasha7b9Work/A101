// 2023/10/20 14:27:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Nextion.h"


namespace Ampermeter
{
    namespace Set
    {
        namespace ZeroDC
        {
            static float value = 0.0f;
        }

        namespace ZeroAC
        {
            static float value = 0.0f;
        }
    }
}


void Ampermeter::Set::ZeroDC::Enable()
{
    value = 0.0f;

    Measure measure = Ampermeter::GetDC();

    value = measure.IsValid() ? measure.value : 0.0f;
}


void Ampermeter::Set::ZeroDC::Disable()
{
    value = 0.0f;

    Nextion::SetVisible("t5", false);

    Nextion::SetValue("bt11", 0);
}


float Ampermeter::Set::ZeroDC::Level()
{
    return value;
}


void Ampermeter::Set::ZeroAC::Enable()
{
    value = 0.0f;

    Measure measure = Ampermeter::GetAC();

    value = measure.IsValid() ? measure.value : 0.0f;
}


void Ampermeter::Set::ZeroAC::Disable()
{
    value = 0.0f;

    Nextion::SetVisible("t6", false);

    Nextion::SetValue("bt10", 0);
}


float Ampermeter::Set::ZeroAC::Level()
{
    return value;
}
