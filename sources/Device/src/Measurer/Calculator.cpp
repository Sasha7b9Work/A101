// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Calculator.h"
#include <cmath>


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
    return std::fabsf(data.MaxReal() - data.MinReal());
}


float Calculator::GetDC()
{
    return (data.MaxReal() + data.MinReal()) / 2.0f;
}
