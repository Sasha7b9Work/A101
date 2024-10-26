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


#define RETURN_VALUE(arr)                               \
    *correct = arr.NumElements() > 0;                   \
    REAL relative = (*correct) ? arr.Get() : 0.0;       \
    return relative * (Range::Current() > 2 ? 1e3 : 1.0)


#define GET_VALUE(arr)                                                              \
    bool correct = arr.NumElements() > 0;                                           \
    REAL value = (correct ? arr.Get() : 0.0) * (Range::Current() > 2 ? 1e3 : 1.0)


namespace Calculator
{
#define NUM_AVERAGES 1

    // Значения, приведённые к пределу - mA для 2,20,200мА, амперы для 2A, 20A, 50A
    static AveragerReal<NUM_AVERAGES> dc;
    static AveragerReal<NUM_AVERAGES> ac;
    static AveragerReal<NUM_AVERAGES> min;
    static AveragerReal<NUM_AVERAGES> max;
    static AveragerReal<NUM_AVERAGES> ampl;
    static AveragerReal<NUM_AVERAGES> frequency;
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


Measure Calculator::GetMeasureFrequency()
{
    bool correct = frequency.NumElements() > 0;

    REAL value = correct ? frequency.Get() : 0.0;

    return Measure(Measure::LimitFrequency(value), Ampermeter::OutOfRange(), correct);
}


REAL Calculator::GetAbsAC(bool *correct)
{
    RETURN_VALUE(ac);
}


Measure Calculator::GetMeasureAC()
{
    GET_VALUE(ac);

    REAL zero = Ampermeter::ZeroAC::LevelAbs();

    if (Range::Current() > 2)
    {
        zero /= 1e3;
    }

    return Measure(value - zero, Ampermeter::OutOfRange(), correct);
}


REAL Calculator::GetAbsDC(bool *correct)
{
    RETURN_VALUE(dc);
}


Measure Calculator::GetMeasureDC()
{
    GET_VALUE(dc);

    REAL zero = Ampermeter::ZeroDC::LevelAbsFull();

    if (Range::Current() > 2)
    {
        zero /= 1e3;
    }

    return Measure(value - zero, Ampermeter::OutOfRange(), correct);
}


REAL Calculator::GetValueMin(bool *correct)
{
    RETURN_VALUE(min);
}


Measure Calculator::GetMeasureMin()
{
    bool correct = false;

    REAL value = Calculator::GetValueMin(&correct);

    REAL zero = Ampermeter::ZeroDC::LevelAbsFull();

    if (Range::Current() > 2)
    {
        zero /= 1e3;
    }

    return Measure(value - zero, Ampermeter::OutOfRange(), correct);
}


REAL Calculator::GetValueMax(bool *correct)
{
    RETURN_VALUE(max);
}


Measure Calculator::GetMeasureAmpl()
{
    GET_VALUE(ampl);

    return Measure(value, Ampermeter::OutOfRange(), correct);
}


Measure Calculator::GetMeasurePeak()
{
    bool correct_max = false;
    bool correct_min = false;

    REAL value_max = Calculator::GetValueMax(&correct_max);

    REAL value_min = Calculator::GetValueMin(&correct_min);

    return Measure(value_max - value_min, Ampermeter::OutOfRange(), correct_min && correct_max);
}
