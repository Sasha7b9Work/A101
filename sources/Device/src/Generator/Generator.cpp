// 2022/11/01 11:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Ampermeter/AD7691.h"


namespace Generator
{
    static bool enabled = false;
}


void Generator::Enable()
{
    enabled = true;

    AD7691::GeneratorChangedEvent();
}


void Generator::Disable()
{
    enabled = false;

    AD7691::GeneratorChangedEvent();
}


void Generator::SetFrequency(float)
{

}


void Generator::SetPicAC(float)
{

}


void Generator::SetDC(float)
{

}


ValueADC Generator::ReadValue()
{
    return ValueADC(0);
}


bool Generator::IsEanbled()
{
    return enabled;
}
