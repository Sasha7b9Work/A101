// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/ResolverFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Hardware/Timer.h"
#include <cmath>


namespace Calculator
{
    static int num_averages = 0;
//    static Averager<float, 1> dc;
//    static Averager<float, 1> ac;

    static float dc = 0.0f;
    static float ac = 0.0f;

    static float k = 1.0f;
}


void Calculator::Reset(int range)
{
//    dc.Reset();
//    ac.Reset();

    static const float koeff[6] = { 1e-2f, 1e-1f, 1e0f, 1e-2f, 1e-1f, 1e0f };

    k = koeff[range] * 61.81f;
}


SampleRate Calculator::AppendData()
{
//    TimeMeterMS meter;

    Period period = ResolverPeriodSamples().GetResult();

    float value_ac = ResolverAC(period).GetResult();

//    LOG_WRITE("period %d, ac %f : %f, time %d ms", period.Lenght(), (double)value_ac, (double)(value_ac * k), meter.ElapsedTime());

    ac = value_ac * k;

    dc = period.dc.Real() * k;

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
