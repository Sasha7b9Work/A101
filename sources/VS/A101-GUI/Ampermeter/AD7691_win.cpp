// 2023/11/09 12:21:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"


SampleRate SampleRate::current(10);


ValueADC::ValueADC(int)
{

}


void AD7691::Init()
{

}


ValueADC AD7691::ReadValue()
{
    return ValueADC(0);
}


int AD7691::GetAverageValue()
{
    return 0;
}


void AD7691::ResetValue()
{

}
