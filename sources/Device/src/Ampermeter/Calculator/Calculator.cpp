// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/FFT.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include "Ampermeter/Calculator/Averager.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
    static Averager<double, 64> dc;
    static Averager<double, 64> ac;

    static BufferADC data;

    static double CalculateAC(int period, double dc_val);
    static double CalculateDC(int period);

    static double k = 1.0;
}


void Calculator::Reset(int range)
{
    dc.Reset();
    ac.Reset();

    static const double koeff[6] = { 1e-2, 1e-1, 1e0, 1e-2, 1e-1, 1e0 };

    k = koeff[range] * 61.79;
}


SampleRate Calculator::AppendData(const BufferADC &_data)
{
    data = _data;

    int period = PeriodInt(data).ToPoints();

    double dc_val = CalculateDC(period);

    dc.Push(dc_val * k);

    ac.Push(CalculateAC(period, dc_val) * k);

    FFT fft(data);

    return SampleRate::Current::Get();
}


double Calculator::GetAC()
{
    return ac.Get();
}


double Calculator::GetDC()
{
    return dc.Get();
}


void Calculator::SetAverages(int num_ave)
{
    num_averages = num_ave;
}


double Calculator::CalculateAC(int period, double dc_val)
{
    double sum = 0.0;

    for (uint i = 0; i < (uint)period; i++)
    {
        double value = data[(int)i].Real() - dc_val;

        sum += value * value;
    }

    return std::sqrt(sum / period);
}


double Calculator::CalculateDC(int period)
{
    double sum = 0.0;

    for (int i = 0; i < period; i++)
    {
        sum += data[i].Real();
    }

    return sum / (double)period;
}
