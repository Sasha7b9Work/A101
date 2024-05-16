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


namespace Calculator
{
    static const int SIZE = 24;
    static const int NUM_VALUES = 1;

    static REAL vals_ac[SIZE] = { 13.37, 1.9, 2.1, 19.0, 21.0, 190.0, 210.0, 1.9, 2.1, 19.0, 21.0, 49.0, 51.0 };
    static REAL vals_dc[SIZE] = { 13.37, 1.9, 2.1, 19.0, 21.0, 190.0, 210.0, 1.9, 2.1, 19.0, 21.0, 49.0, 51.0 };

    static const uint tempo_secs = 1;          // Так часто будем переключать измерения
}


REAL Calculator::GetAC(bool *correct)
{
#ifdef EMULATOR_ENABLED

    static int pointer = 0;
    static uint next_secs = 0;

    if (Timer::GetSecs() == next_secs)
    {
        next_secs += tempo_secs;

        if (++pointer == NUM_VALUES)
        {
            pointer = 0;
        }
    }

    *correct = true;

    return CastRealToDisplay(vals_ac[pointer], Range::Current());

#else

    *correct = (ac.NumElements() > 0);

    return ac.NumElements() ? ac.Get() : 0.0;

#endif
}


REAL Calculator::GetDC(bool *correct)
{
#ifdef EMULATOR_ENABLED

    *correct = true;

    static int pointer = 0;
    static uint next_secs = 0;

    if (Timer::GetSecs() == next_secs)
    {
        next_secs += tempo_secs;

        if (++pointer == NUM_VALUES)
        {
            pointer = 0;
        }
    }

    return CastRealToDisplay(vals_dc[pointer], Range::Current());

#else

    *correct = (dc.NumElements() > 0);

    return dc.NumElements() ? dc.Get() : 0.0;

#endif
}


REAL Calculator::CastRealToDisplay(REAL value, int range)
{
    if (range < 3)
    {
        return value * 1e3;
    }

    return value;
}
