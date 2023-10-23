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
#include <cmath>


namespace Calculator
{
    static Averager<double, 1> dc;
    static Averager<double, 1> ac;
}


void Calculator::Reset()
{
    dc.Reset();
    ac.Reset();
}


SampleRate Calculator::AppendData()
{
    Period period = ResolverPeriodSamples().GetResult();

    double value_ac = ResolverAC(period).GetResult();

    double k = cal.gain[Range::Current()].Get();

    ac.Push(value_ac * k);

    double value_dc = -period.dc.Real();

    LOG_WRITE_TRACE("value_dc = %f %f", value_dc, value_dc * k);

    dc.Push(value_dc * k);

    return SampleRate::Current::Get();
}


double Calculator::GetAC(bool *correct)
{
    *correct = (ac.NumElements() > 0);

    return ac.NumElements() ? ac.Get() : 0.0;
}


double Calculator::GetDC(bool *correct)
{
    *correct = (dc.NumElements() > 0);

    return dc.NumElements() ? dc.Get() : 0.0;
}
