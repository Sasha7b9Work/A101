// 2023/10/20 14:27:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"


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
    Measure measure = Ampermeter::GetDC();

    value = measure.IsValid() ? measure.value : 0.0f;
}


void Ampermeter::Set::ZeroDC::Disable()
{
    value = 0.0f;
}


float Ampermeter::Set::ZeroDC::Level()
{
    return value;
}


void Ampermeter::Set::ZeroAC::Enable()
{
    Measure measure = Ampermeter::GetAC();

    value = measure.IsValid() ? measure.value : 0.0f;
}


void Ampermeter::Set::ZeroAC::Disable()
{
    value = 0.0f;
}


float Ampermeter::Set::ZeroAC::Level()
{
    return value;
}
