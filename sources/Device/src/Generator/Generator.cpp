// 2022/11/01 11:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Measurer/AD7691.h"


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


void Generator::SetAC(float)
{

}


void Generator::SetDC(float)
{

}


int Generator::ReadValue()
{
    return 0;
}


bool Generator::IsEanbled()
{
    return enabled;
}
