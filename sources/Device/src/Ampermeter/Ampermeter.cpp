// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/AD7691.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Display/Indicator.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/Calibrator.h"
#include "Hardware/Timer.h"
#include "Display/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Settings/Settings.h"
#include "Ampermeter/FIR.h"
#include "stm_includes.h"
#include "Menu/Pages/Pages.h"
#include <cmath>


namespace Ampermeter
{
    // ��������� ������ �� ���
    struct MiddlerOf3
    {
        void Push(ValueADC value)
        {
            averager.Push(value);
        }
        ValueADC Get()
        {
            if (averager.NumElements() > 2)
            {
                return ValueADC::FromRaw(middle_of_3(averager.Pop(0), averager.Pop(1), averager.Pop(2)));
            }
            else if (averager.NumElements() == 2)
            {
                return averager.Pop(1);
            }

            return averager.Pop(0);
        }

    private:

        Averager <ValueADC, 3> averager;

        int middle_of_3(int a, int b, int c)
        {
            if ((a <= b) && (a <= c))
            {
                return (b <= c) ? b : c;
            }
            else if ((b <= a) && (b <= c))
            {
                return (a <= c) ? a : c;
            }
            return (a <= b) ? a : b;
        }
    };
}


void Ampermeter::Init()
{
    HAL_TIM4::Init();

    AD7691::Init();

    Indicator::OnEvent::CnageRange();
}


void Ampermeter::Update()
{
    ReadData();

    SampleRate::Current::Set(Calculator::AppendData());

    if (OutOfRange())
    {
        Indicator::SetOverflow();
    }
    else
    {
        Indicator::SetMeasures(Calculator::GetAC());

        DiagramInput::SetData();
    }
}


float Ampermeter::GetDC()
{
    return Calculator::GetDC();
}


void Ampermeter::ReadData()
{
    TimeMeterMS meter;

    BufferADC::Clear(SampleRate::Current::Get());

    uint period = SampleRate::Current::Get().TimeUS();

    HAL_TIM4::StartPeriodicUS(period);

    int num_sample = 0;

    while (!BufferADC::IsFull())
    {
#ifndef WIN32
        while (TIM4->CNT < period)
        {
        }

        TIM4->CNT = 0;
#endif

        ValueADC value = AD7691::ReadValue();

        if (set.firLPF)
        {
            value = ValueADC::FromRaw(FIR::Step(value.Raw()));

            if (num_sample++ > 200)
            {
                BufferADC::Push(value);
            }
        }
        else
        {
            BufferADC::Push(value);
        }
    }

    //    LOG_WRITE("time measure %d ms, time point %f us", meter.ElapsedTime(), (meter.ElapsedTime() / (double)BufferADC::SIZE) * 1e3);

    HAL_TIM4::Stop();

    if (set.middle_of_3)
    {
        BufferADC::MiddleOf3();
    }

    if (set.smooth)
    {
        BufferADC::SmoothOut();
    }

    BufferADC::CalculateLimits();
}


bool Ampermeter::OutOfRange()
{
    static const float maxs[6] = { 2e-3f, 20e-3f, 200e-3f, 2.0f, 20.0f, 50.0f };

    float max = maxs[Range::Current()] * 1.1f * 1e3f;

    float value = std::fabsf(Calculator::GetDC()) + Calculator::GetAC();

    if (value > max)
    {
        LOG_WRITE("out range dc = %f, ac = %f", (double)Calculator::GetDC(), (double)Calculator::GetAC());

        return true;
    }

    return false;
}
