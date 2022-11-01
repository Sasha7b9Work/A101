// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
    static float dc = 0.0f;
    static float ac = 0.0f;
}

SampleRate Calculator::AppendData(const BufferADC &data)
{
    ac = std::fabs(data.MaxReal() - data.MinReal());

    dc = (data.MaxReal() + data.MinReal()) / 2.0f;

    return SampleRate(10);
}


float Calculator::GetAC()
{
    return ac;
}


float Calculator::GetDC()
{
    return dc;
}


void Calculator::SetAverages(int num_ave)
{
    num_averages = num_ave;
}
