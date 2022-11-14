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
#include "Settings.h"
#include <cmath>


namespace Calculator
{
    static float dc = 0.0f;
    static float ac = 0.0f;
}


SampleRate Calculator::AppendData()
{
    Period period = ResolverPeriodSamples().GetResult();

    float value_ac = ResolverAC(period).GetResult();

    float k = set.cal.Gain(InputRelays::GetRange());

    ac = value_ac * k;

    dc = -period.dc.Real() * k;

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
