// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Calculator.h"


void Calculator::AppendData(const BufferADC &)
{

}


float Calculator::GetAC()
{
    static float value = 0.0f;

    value += 1.0f;

    return value;
}


float Calculator::GetDC()
{
    static float value = 0.0f;

    value += 1.1f;

    return value;
}
