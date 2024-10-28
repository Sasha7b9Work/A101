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
    static AveragerReal<NUM_AVERAGES> peak;
    static AveragerReal<NUM_AVERAGES> frequency;
}


void Calculator::Reset()
{
    dc.Reset();
    ac.Reset();
    min.Reset();
    max.Reset();
    ampl.Reset();
    peak.Reset();
    frequency.Reset();
}


void Calculator::AppendData()
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

    // Считаем все измерения
    {
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

        frequency.Push(freq);

#endif
        ResolverMeasures resolver(period, freq);

        peak.Push(resolver.GetPeak() * k);

        min.Push(resolver.GetMin() * k);

        max.Push(resolver.GetMax() * k);

        ampl.Push(resolver.GetAmplitude() * k);
    }
}


Measure Calculator::GetMeasureFrequency()
{
    bool correct = frequency.NumElements() > 0;

    REAL value = correct ? frequency.Get() : 0.0;

    return Measure(value, OutOfRange(), correct);
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

    return Measure(value - zero, OutOfRange(), correct);
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

    return Measure(value - zero, OutOfRange(), correct);
}


REAL Calculator::GetValueMinSteady(bool *correct)
{
    RETURN_VALUE(min);
}


Measure Calculator::GetMeasureMinSteady()
{
    bool correct = false;

    REAL value = GetValueMinSteady(&correct);

    REAL zero = Ampermeter::ZeroDC::LevelAbsFull();

    if (Range::Current() > 2)
    {
        zero /= 1e3;
    }

    return Measure(value - zero, OutOfRange(), correct);
}


REAL Calculator::GetValueMaxSteady(bool *correct)
{
    RETURN_VALUE(max);
}


Measure Calculator::GetMeasureAmplSteady()
{
    GET_VALUE(ampl);

    return Measure(value, OutOfRange(), correct);
}


Measure Calculator::GetMeasurePeak()
{
    GET_VALUE(peak);

    return Measure(value, OutOfRange(), correct);
}


Measure Calculator::GetMeasureMaxSteady()
{
    bool correct = false;

    REAL value_max = GetValueMaxSteady(&correct);

    REAL zero = Ampermeter::ZeroDC::LevelAbsFull();

    if (Range::Current() > 2)
    {
        zero /= 1e3;
    }

    return Measure(value_max - zero, OutOfRange(), correct);
}


bool Calculator::OutOfRange()
{
    REAL value_max = Measure::MaxIAbs(Range::Current()) * 1.15;

    bool correct_dc = false;
    bool correct_ac = false;

    REAL value_dc = Calculator::GetAbsDC(&correct_dc);
    REAL value_ac = Calculator::GetAbsAC(&correct_ac);

    REAL value = std::fabs(value_dc) + value_ac;

    return value > value_max;
}
