// 2023/10/20 14:27:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/InputRelays.h"
#include "Menu/Pages/Pages.h"


namespace Ampermeter
{
    namespace Set
    {
        namespace ZeroDC
        {
            static REAL value_abs = 0.0;
        }

        namespace ZeroAC
        {
            static REAL value_abs = 0.0;
        }
    }
}


void Ampermeter::Set::ZeroDC::Enable()
{
    AVP::Disable();

    value_abs = 0.0;

    Measure measure = Ampermeter::GetDC();

    value_abs = measure.IsValid() ? measure.value_abs : 0.0;

    if (Range::Current() > 2)
    {
        value_abs *= 1e3;
    }

    PageMain::HightLightCurrentRange();
}


void Ampermeter::Set::ZeroDC::_Disable()
{
    value_abs = 0.0;

    Nextion::SetVisible("tzDC", false);

    Nextion::SetValue("bt11", 0);
}


REAL Ampermeter::Set::ZeroDC::LevelAbs()
{
    return value_abs;
}


void Ampermeter::Set::ZeroAC::Enable()
{
    AVP::Disable();

    value_abs = 0.0;

    Measure measure = Ampermeter::GetAC();

    value_abs = measure.IsValid() ? measure.value_abs : 0.0;

    if (Range::Current() > 2)
    {
        value_abs *= 1e3;
    }

    PageMain::HightLightCurrentRange();
}


void Ampermeter::Set::ZeroAC::_Disable()
{
    value_abs = 0.0;

    Nextion::SetVisible("tzAC", false);

    Nextion::SetValue("bt10", 0);
}


REAL Ampermeter::Set::ZeroAC::LevelAbs()
{
    return value_abs;
}
