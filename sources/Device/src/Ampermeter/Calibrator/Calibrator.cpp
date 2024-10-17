// 2022/11/01 12:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/InputRelays.h"
#include "Utils/Math.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Ampermeter/AD7691.h"
#include <cstdio>
#include <cmath>


namespace Calibrator
{
    struct CalibratorZero
    {
        CalibratorZero(int _range) : range(_range) {}
        bool Run();
    private:
        int range;
        REAL CalculateDC(int zero);
    };

    static void (*callbackUpdate)() = nullptr;

    // Откалибровать усиление
    static bool CalibrateGain(int range);

    static bool in_progress = false;

    bool InProgress()
    {
        return in_progress;
    }
}


bool Calibrator::Run(int range, Type::E type, void (*callback)())
{
    in_progress = true;

    callbackUpdate = callback;

    Range::Set(range);

    TimeMeterMS().Wait(1000);

    bool result = false;

    if (type == Type::DC)
    {
        result = CalibratorZero(range).Run();
    }
    else if (type == Type::AC)
    {
        result = CalibrateGain(range);
    }

    in_progress = false;

    return result;
}


bool Calibrator::CalibratorZero::Run()
{
    CalibrationSettings::Zero &zero = cal.zero[Range::Current()];

    const int const_val = zero.GetFull();

    InputRelays::EnableZero(true);
    zero.SetVar(0);
    zero.SetConst(0);
    int average = AD7691::GetAverageValue();
    LOG_WRITE("average = %d", average);
    zero.SetVar(average);
    InputRelays::DisableZero(true);

    zero.SetConst(const_val);

    REAL dc = CalculateDC(0);

#ifdef LOGGED
    REAL dc1000 = CalculateDC(1000);

    LOG_WRITE("dc = %f, dc1000 = %f", (double)dc, (double)dc1000);
#endif

    int z = 0;

    int delta = (dc < 0.0) ? 1000 : -1000;

    {
        for (int i = 0; i < 4; i++)
        {
            REAL prev_dc = dc;

            while ((int)Math::Sign(prev_dc) == (int)Math::Sign(dc))
            {
                prev_dc = dc;

                dc = CalculateDC(z);

                LOG_WRITE("z = %d, dc = %f", z, (double)dc);

                if (std::fabs(dc) < 1e-6)
                {
                    i = 5;
                    break;
                }

                callbackUpdate();

                z += delta;
            }

            delta = -delta / 10;
        }
    }

#ifdef LOGGED
    bool correct_ac = false;

    LOG_WRITE("z - 1 = %d, dc = %f", z - 1, (double)CalculateDC(z - 1));
    LOG_WRITE("z = %d, ac = %e, dc = %e", z, (double)Calculator::GetAbsAC(&correct_ac), (double)dc);
    LOG_WRITE("z + 1 = %d, dc = %f", z + 1, (double)CalculateDC(z + 1));
#endif

    bool result = false;

    if (Math::Abs(z) < 10000)
    {
        cal.zero[range].SetConst(z);

        result = true;
    }
    else
    {
        cal.zero[range].SetConst(const_val);
    }

    return result;
}


REAL Calibrator::CalibratorZero::CalculateDC(int zero)
{
    cal.zero[range].SetConst(zero);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    return Calculator::GetAbsDC(&correct_dc);
}


bool Calibrator::CalibrateGain(int range)
{
    cal.gain[range].Set(1.0);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    REAL dc = std::fabs(Calculator::GetAbsDC(&correct_dc));

    REAL k = Range::Max(range) / dc;

    cal.gain[range].Set(k);

    LOG_WRITE("range = %d, dc = %e, k = %e", range, (double)dc, (double)k);

    return true;
}


void Calibrator::PressButtonSave()
{
    cal.Save();
}
