// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/AD7691.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Nextion/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/Timer.h"
#include "Nextion/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Settings/Settings.h"
#include "Ampermeter/FIR.h"
#include "stm_includes.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Utils/String.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cmath>
#include <cstdio>


namespace Ampermeter
{
    namespace AVP
    {
        static bool is_enabled = false;

        void Enable()
        {
            ZeroAC::Disable();
            ZeroDC::Disable();

            is_enabled = true;

            PageMain::btnAVP.SetToggled(true, false);
        }

        void Disable()
        {
            is_enabled = false;
        }
    }

    // Измеряемая величина слишком маленькая - нужно перейти на предыдущий диапазон
    static bool VerySmall();

    // Подстрока нуля
    static void AdjustmentZero();

    static void DrawProgress(TimeMeterMS &timer, uint &prev_time);
}


void Ampermeter::Init()
{
    HAL_TIM4::Init();

    AD7691::Init();
}


void Ampermeter::Update()
{
    if (Page::Current() != PageMain::self && Page::Current() != PageGraph::self && Page::Current() != PageCalibration::self)
    {
        return;
    }

    if (Page::Current() == PageGraph::self &&   // Если страница отрисовки графиков
        DiagramInput::DataInstalled())          // и данные для отрисовки имеются в наличии
    {
        return;                                 // обновляться не будем, потому что это занимает много времени
    }

    AdjustmentZero();

    if (!MeasurementCycle())
    {
        return;
    }

    Calculator::AppendData();

    DiagramInput::InstallData();

    if (AVP::is_enabled)
    {
        int range = Range::Current();

        int min = 4;
        int max = 5;

        if (range < 4)
        {
            min = 0;
            max = 3;
        }

        if (Calculator::OutOfRange())
        {
            if (range < max)
            {
                PageMain::SetRange(max);
            }
        }
        else if (VerySmall())
        {
            if (range > min)
            {
                PageMain::SetRange(range - 1); 
            }
        }

        AVP::Enable();  // Нужно заново включить, потому что нажатие на кнопку отлкючает АВП
    }
}


void Ampermeter::DrawProgress(TimeMeterMS &meter, uint &prev_time)
{
    static bool star_allow = true;          // Разрешена передача звезды
    static bool progress_allow = true;      // Разрешена передача прогресс-бара

    if (SampleRate::TimeFullRead() > meter.ElapsedTime())
    {
        uint time = SampleRate::TimeFullRead() - meter.ElapsedTime();

        if ((prev_time - time) > 50 && Page::Current() == PageMain::self)
        {
            if (star_allow && progress_allow)
            {
                if (HAL_USART2::TransferITAllowed())
                {
                    PageMain::Star::Draw();

                    star_allow = false;
                }
            }

            if (!star_allow && progress_allow)
            {
                if (HAL_USART2::TransferITAllowed())
                {
                    if (SampleRate::Get() != SampleRate::_10us)
                    {
                        static char buffer[64];

                        sprintf(buffer, "xstr 520,5,70,27,0,65535,6964,0,0,1,\"%s\"\xFF\xFF\xFF", String<>("%3.1f", time / 1000.0f).c_str());

                        HAL_USART2::TransmitIT(buffer);
                    }

                    progress_allow = false;
                }
            }

            if (!star_allow && !progress_allow)
            {
                prev_time = time;
                star_allow = true;
                progress_allow = true;
            }
        }
    }
}


bool Ampermeter::MeasurementCycle()
{
    if (Page::Current() == PageMain::self)
    {
        Nextion::DrawString({ 520, 5, 50, 27 }, Font::_0_GB34b, Color::White, Color::Background, "", false, false);
    }

    TimeMeterMS meter;

    BufferADC::Clear();

#ifndef WIN32

    volatile uint period = SampleRate::TimeUSonPoint();

#endif

    HAL_TIM4::Start();

    int num_samples = 0;

    uint prev_time = SampleRate::TimeFullRead();

    while (!BufferADC::IsFull())
    {
        DrawProgress(meter, prev_time);

        int counter_raw = 0;
        int64 sum_raw = 0;

#ifndef WIN32

        do
        {
            counter_raw++;
            sum_raw += AD7691::ReadValueRAW();

            if (!Calibrator::InProgress() && Nextion::ExixtCommnadsForExecute())
            {
                HAL_TIM4::Stop();

                while (!HAL_USART2::TransferITAllowed())
                {
                }

                return false;
            }

        } while(TIM4->CNT < period * 2 / 3);

        while (TIM4->CNT < period)
        {
        }

        TIM4->CNT = 0;

#else

        for (int i = 0; i < 1; i++)
        {
            counter_raw++;
            sum_raw += AD7691::ReadValueRAW();
        }

#endif

        ValueADC value((int)((float)sum_raw / (float)counter_raw + 0.5f));

        if (set.firLPF)
        {
            value = ValueADC::FromRaw(FIR::Step(value.Raw()));

            if (num_samples++ > 200)
            {
                BufferADC::Push(value);
            }
        }
        else
        {
            BufferADC::Push(value);
            num_samples++;
        }
    }

    HAL_TIM4::Stop();

    BufferADC::MiddleOf3();

    if (set.smooth)
    {
        BufferADC::SmoothOut();
    }

    BufferADC::CalculateLimits();

    while (!HAL_USART2::TransferITAllowed())
    {
    }

    return true;
}


REAL Measure::MaxIAbs(int range)
{
    static const REAL maxs[6] = { 2.0, 2e1, 2e2, 2e3, 2e4, 5e4 };

    return maxs[range];
}


bool Ampermeter::VerySmall()
{
    static const REAL mins[6] = { 0.0, 2e0, 2e1, 2e2, 2e3, 20e3 };

    REAL min = mins[Range::Current()];

    bool correct_dc = false;
    bool correct_ac = false;

    REAL dc = Calculator::GetAbsDC(&correct_dc);
    REAL ac = Calculator::GetAbsAC(&correct_ac);

    if (!correct_ac || !correct_dc)
    {
        return false;
    }

    REAL value = std::fabs(dc) + ac;

    return value <= min;
}


void Ampermeter::OnEventChangeRange()
{
    Calculator::Reset();
}


void Ampermeter::AdjustmentZero()
{
    static int prev_range = -1;

    static uint next_time = 0;

    static bool need_pause = true;

    if (Range::Current() == prev_range)
    {
        need_pause = true;

        if (TIME_MS < next_time)
        {
            return;
        }
    }
    else
    {
        if (need_pause)
        {
            need_pause = false;

            if (Range::Current() > 3)
            {
                Timer::Delay(400);
            }
        }
    }

    next_time = TIME_MS + 10000;

    prev_range = Range::Current();

    CalibrationSettings::Zero &zero = cal.zero[Range::Current()];
    const int const_val = zero.GetConst();

    zero.SetConst(0);
    zero.SetVar(0);

    InputRelays::EnableZero(false);

    int zero_var = AD7691::GetAverageValue();

    zero.SetVar(zero_var);

    InputRelays::DisableZero(false);

    zero.SetConst(const_val);

    HAL_PIO::Write(PIN_ZERO, false);
}
