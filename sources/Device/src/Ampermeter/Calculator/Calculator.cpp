// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Hardware/Timer.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"
#include "Nextion/Display.h"
#include <cmath>


namespace Calculator
{
#define NUM_AVERAGES 1

    static AveragerReal<NUM_AVERAGES> dc;
    static AveragerReal<NUM_AVERAGES> ac;
    static AveragerReal<NUM_AVERAGES> min;
    static AveragerReal<NUM_AVERAGES> max;
    static AveragerReal<NUM_AVERAGES> ampl;
    static AveragerReal<NUM_AVERAGES> frequency;

    // Значения, приведённые к пределу - mA для 2,20,200мА, амперы для 2A, 20A, 50A
    static REAL GetRelativeDC(bool *correct);
    static REAL GetMin(bool *correct);
    static REAL GetMax(bool *correct);
    static REAL GetAmpl(bool *correct);
    static REAL GetFrequency(bool *correct);
}


void Calculator::Reset()
{
    dc.Reset();
    ac.Reset();
    min.Reset();
    max.Reset();
    ampl.Reset();
    frequency.Reset();
}


SampleRate Calculator::AppendData()
{
    Period period = ResolverPeriodSamples().GetResult();

    const REAL k = cal.gain[Range::Current()].Get();

    // Считаем AC
    {
        REAL value_ac = ResolverAC(period).GetResult();

        ac.Push(value_ac * k);
    }

    // Считаем ВС
    {
        REAL value_dc = -period.dc.Real();

        dc.Push(value_dc * k);
    }

    // Считаем MIN и MAX и AMPL
    {
        ResolverMinMax resolver_min_max(period);

        min.Push(resolver_min_max.GetMin() * k);

        max.Push(resolver_min_max.GetMax() * k);

        ResolverAmpl resolver_ampl(period, resolver_min_max.GetMin(), resolver_min_max.GetMax());

        REAL ampl_value = resolver_ampl.GetResult();

        ampl.Push(ampl_value * k);
    }

#if 0

    if (ampl.Get() > Range::Max(Range::Current()) * 0.1)        // Частоту выводим только если амплитуда превышает 10% от максимального значения
    {
        frequency.Push(ResolverFrequency(period).GetFrequency());
    }
    else
    {
        frequency.Reset();
    }

#else

    REAL freq = ResolverFrequency(period).GetFrequency();

    if (freq <= 5e3)
    {
        frequency.Push(ResolverFrequency(period).GetFrequency());
    }
    else
    {
        frequency.Reset();
    }

#endif

    return SampleRate::Current::Get();
}


REAL Calculator::GetRelativeDC(bool *correct)
{
    *correct = (dc.NumElements() > 0);

    return dc.NumElements() ? dc.Get() : 0.0;
}


REAL Calculator::GetMin(bool *correct)
{
    *correct = (min.NumElements() > 0);

    return min.NumElements() ? min.Get() : 0.0;
}


REAL Calculator::GetFrequency(bool *correct)
{
    *correct = (frequency.NumElements() > 0);

    return frequency.NumElements() ? frequency.Get() : 0.0;
}


REAL Calculator::GetMax(bool *correct)
{
    *correct = (max.NumElements() > 0);

    return max.NumElements() ? max.Get() : 0.0;
}


REAL Calculator::GetAmpl(bool *correct)
{
    *correct = (ampl.NumElements() > 0);

    return ampl.NumElements() ? ampl.Get() : 0.0;
}


REAL Calculator::GetAbsAC(bool *correct)
{
    *correct = (ac.NumElements() > 0);

    REAL relative_ac = *correct ? ac.Get() : 0.0;

    return relative_ac * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetAbsDC(bool *correct)
{
    return GetRelativeDC(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetValueMin(bool *correct)
{
    return GetMin(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetValueMax(bool *correct)
{
    return GetMax(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetValueAmpl(bool *correct)
{
    return GetAmpl(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetValueFrequency(bool *correct)
{
    return GetFrequency(correct);
}
