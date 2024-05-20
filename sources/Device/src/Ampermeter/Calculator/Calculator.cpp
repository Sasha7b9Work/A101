// 2022/10/19 08:16:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calculator/ResolverFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodFFT.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Hardware/Timer.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include <cmath>


namespace Calculator
{
    static Averager<REAL, 1> dc;
    static Averager<REAL, 1> ac;

    // Значения, приведённые к пределу - mA для 2,20,200мА, амперы для 2A, 20A, 50A
    REAL GetRelativeDC(bool *correct);
    REAL GetRelativeAC(bool *correct);
}


void Calculator::Reset()
{
    dc.Reset();
    ac.Reset();
}


SampleRate Calculator::AppendData()
{
    Period period = ResolverPeriodSamples().GetResult();

    REAL value_ac = ResolverAC(period).GetResult();

    const REAL k = cal.gain[Range::Current()].Get();

    REAL value = value_ac * k;

    ac.Push(value);

    REAL value_dc = -period.dc.Real();

    value = value_dc * k;

    dc.Push(value);

    Display::LabelStar::Show();

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


REAL Calculator::GetAbsAC(bool *correct)
{
    return GetRelativeAC(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}


REAL Calculator::GetAbsDC(bool *correct)
{
    return GetRelativeDC(correct) * (Range::Current() > 2 ? 1e3 : 1.0);
}
