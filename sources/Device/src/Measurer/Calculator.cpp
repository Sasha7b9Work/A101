// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Calculator.h"


namespace Calculator
{
    static BufferADC data;
}


void Calculator::AppendData(const BufferADC &_data)
{
    data = _data;
}


float Calculator::GetAC()
{
}


float Calculator::GetDC()
{
    static float value = 0.0f;

    value += 1.1f;

    return value;
}
