// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/FFT.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
    static double dc = 0.0f;
    static double ac = 0.0f;

    static BufferADC data;

    static double CalculateAC(int period);
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


double Calculator::GetAC()
{
    return ac;
}


double Calculator::GetDC()
{
    return dc;
}


void Calculator::SetAverages(int num_ave)
{
    num_averages = num_ave;
}


double Calculator::CalculateAC(int period)
{
    double sum = 0.0f;

    for (int i = 0; i < period; i++)
    {
        sum += data[i].Real() * data[i].Real();
    }

    return std::sqrt(sum / period);
}
