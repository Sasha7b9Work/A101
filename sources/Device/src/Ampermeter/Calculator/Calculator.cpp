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
#include <cmath>


namespace Calculator
{
    static float dc = 0.0f;
    static float ac = 0.0f;

    static float k = 1.0f;
}


void Calculator::OnChangeRangeEvent()
{
    static const float koeff[6] = { 1e-2f, 1e-1f, 1e0f, 1e-2f, 1e-1f, 1e0f };

    k = koeff[InputRelays::GetRange()] * 61.81f;
}


SampleRate Calculator::AppendData()
{
    Period period = ResolverPeriodSamples().GetResult();

    float value_ac = ResolverAC(period).GetResult();

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
