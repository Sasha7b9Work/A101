// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator.h"
#include <cmath>


namespace Calculator
{
    static BufferADC data;
    static int num_averages = 0;

    // Расчёт частоты сигнала. sample_rate - частота дискретизации
    float CalculateFrequency(const BufferADC &, float sample_rate);
}


void Calculator::AppendData(const BufferADC &_data)
{
    data = _data;
}


float Calculator::GetAC()
{
    return std::fabs(data.MaxReal() - data.MinReal());
}


float Calculator::GetDC()
{
    return (data.MaxReal() + data.MinReal()) / 2.0f;
}


void Calculator::SetAverages(int num_ave)
{
    num_averages = num_ave;
}
