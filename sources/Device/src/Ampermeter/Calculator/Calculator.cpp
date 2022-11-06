// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/FFT.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
    static float dc = 0.0f;
    static float ac = 0.0f;

    static BufferADC data;

    static float CalculateAC(int period);
}

SampleRate Calculator::AppendData(const BufferADC &_data)
{
    data = _data;

    int period = PeriodInt(data).ToPoints();

    ac = CalculateAC(period);

    dc = (data.MaxReal() + data.MinReal()) / 2.0f;

    FFT fft(data);

    return SampleRate::Current::Get();
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


float Calculator::CalculateAC(int period)
{
    return 0.0f;
}
