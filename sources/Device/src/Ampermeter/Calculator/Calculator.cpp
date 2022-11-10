// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/ResolverFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Ampermeter/Calculator/ResolverDC.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Hardware/Timer.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
    static Averager<float, 1> dc;
    static Averager<float, 1> ac;

    static float k = 1.0f;
}


void Calculator::Reset(int range)
{
    dc.Reset();
    ac.Reset();

    static const float koeff[6] = { 1e-2f, 1e-1f, 1e0f, 1e-2f, 1e-1f, 1e0f };

    k = koeff[range] * 61.81f;
}


SampleRate Calculator::AppendData(const BufferADC &data)
{
    TimeMeterMS meter;

    Period period = ResolverPeriodSamples(data).GetResult();

    float value_ac = ResolverAC(data, period).GetResult();

    LOG_WRITE("period %d, ac %f : %f, time %d ms", period.Lenght(), (double)value_ac, (double)(value_ac * k), meter.ElapsedTime());

    ac.Push(value_ac * k);

    return SampleRate::Current::Get();
}


//SampleRate Calculator::AppendData(const BufferADC &data)
//{
//    double dc_val = ResolverDC::Calculate(data, period);
//
//    dc.Push(dc_val * k);
//}


float Calculator::GetAC()
{
    return ac.Get();
}


float Calculator::GetDC()
{
    return dc.Get();
}


void Calculator::SetAverages(int num_ave)
{
    num_averages = num_ave;
}
