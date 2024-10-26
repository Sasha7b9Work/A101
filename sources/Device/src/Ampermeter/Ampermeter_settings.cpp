// 2023/10/20 14:27:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/InputRelays.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calculator/Calculator.h"


namespace Ampermeter
{
    namespace ZeroDC
    {
        namespace FloatingZero
        {
            static REAL value_abs[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
        }

        static REAL value_abs = 0.0;
    }

    namespace ZeroAC
    {
        static REAL value_abs = 0.0;
    }
}


void Ampermeter::ZeroDC::Enable()
{
    AVP::Disable();

    value_abs = 0.0;

    Measure measure = Calculator::GetMeasureDC();

    value_abs = measure.IsValid() ? measure.value_abs : 0.0;

    if (Range::Current() > 2)
    {
        value_abs *= 1e3;
    }

    PageMain::HightLightCurrentRange();
}


void Ampermeter::ZeroDC::FloatingZero::Process()
{
    value_abs[Range::Current()] = 0.0;

    Measure measure = Calculator::GetMeasureDC();

    value_abs[Range::Current()] = measure.IsValid() ? measure.value_abs : 0.0;

    if (Range::Current() > 2)
    {
        value_abs[Range::Current()] *= 1e3;
    }
}


void Ampermeter::ZeroDC::Disable()
{
    value_abs = 0.0;
    PageMain::btnZeroDC.SetToggled(false, false);
}


REAL Ampermeter::ZeroDC::LevelAbsFull()
{
    return value_abs + FloatingZero::value_abs[Range::Current()];
}


void Ampermeter::ZeroAC::Enable()
{
    AVP::Disable();

    value_abs = 0.0;

    Measure measure = Calculator::GetMeasureAC();

    value_abs = measure.IsValid() ? measure.value_abs : 0.0;

    if (Range::Current() > 2)
    {
        value_abs *= 1e3;
    }

    PageMain::HightLightCurrentRange();
}


void Ampermeter::ZeroAC::Disable()
{
    value_abs = 0.0;
    PageMain::btnZeroAC.SetToggled(false, false);
}


REAL Ampermeter::ZeroAC::LevelAbs()
{
    return value_abs;
}
