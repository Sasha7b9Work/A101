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

    static Averager<REAL, NUM_AVERAGES> dc;
    static Averager<REAL, NUM_AVERAGES> ac;
    static Averager<REAL, NUM_AVERAGES> min;
    static Averager<REAL, NUM_AVERAGES> max;
    static Averager<REAL, NUM_AVERAGES> ampl;
    static Averager<REAL, NUM_AVERAGES> frequency;

    // Значения, приведённые к пределу - mA для 2,20,200мА, амперы для 2A, 20A, 50A
    static REAL GetRelativeDC(bool *correct);
    static REAL GetRelativeAC(bool *correct);
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

    if (ampl.Get() > Range::Max(Range::Current()) * 0.1)        // Частоту выводим только если амплитуда превышает 10% от максимального значения
    {
        frequency.Push(period.GetFrequency());
    }
    else
    {
        frequency.Reset();
    }

    return SampleRate::Current::Get();
}


#ifdef EMULATOR_ENABLED

namespace Calculator
{
    static const uint tempo_secs = 2;   // Так часто будем переключать измерения

    static REAL CastRealToDisplay(REAL value)
    {
        return Range::Current() > 2 ? (value * 1e-3) : (value);
    }
}

#endif


REAL Calculator::GetRelativeAC(bool *correct)
{
#ifdef EMULATOR_ENABLED

    static REAL value = 0.001;
    static REAL k = 1.5;
    static uint next_secs = 0;

    if (Timer::GetSecs() >= next_secs)
    {
        next_secs += tempo_secs;

        value *= k;

        if (value > 10000e3 || value < 0.001)
        {
            k = 1.0 / k;
        }
    }

    *correct = true;

    return CastRealToDisplay(value);

#else

    *correct = (ac.NumElements() > 0);

    return ac.NumElements() ? ac.Get() : 0.0;

#endif
}


REAL Calculator::GetRelativeDC(bool *correct)
{
#ifdef EMULATOR_ENABLED

    *correct = true;

    static REAL value = 0.001;
    static REAL k = 1.5;
    static uint next_secs = 0;

    if (Timer::GetSecs() >= next_secs)
    {
        next_secs += tempo_secs;

        value *= k;

        if (value > 50e3 || value < 0.001)
        {
            k = 1.0f / k;
        }
    }

    return CastRealToDisplay(value);

#else

    *correct = (dc.NumElements() > 0);

    return dc.NumElements() ? dc.Get() : 0.0;

#endif
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
    return GetRelativeAC(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
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
