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
        double CalculateDC(int zero);
    };

    static void (*callbackUpdate)() = nullptr;

    // Откалибровать усиление
    static bool CalibrateGain(int range);
}


bool Calibrator::Run(int range, Type::E type, void (*callback)())
{
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

    return result;
}


bool Calibrator::CalibratorZero::Run()
{
    CalibrationSettings::Zero &zero = cal.zero[Range::Current()];

    const int const_val = zero.GetFull();

    InputRelays::EnableZero();
    zero.SetVar(0);
    zero.SetConst(0);
    int average = AD7691::GetAverageValue();
    LOG_WRITE("average = %d", average);
    zero.SetVar(average);
    InputRelays::DisableZero();

    zero.SetConst(const_val);

    double dc = CalculateDC(0);

    double dc1000 = CalculateDC(1000);

    LOG_WRITE("dc = %f, dc1000 = %f", dc, dc1000);

    int z = 0;

    int delta = (dc < 0.0) ? 1000 : -1000;

    {
        for (int i = 0; i < 4; i++)
        {
            double prev_dc = dc;

            while ((int)Math::Sign(prev_dc) == (int)Math::Sign(dc))
            {
                prev_dc = dc;

                dc = CalculateDC(z);

                LOG_WRITE("z = %d, dc = %f", z, dc);

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

    bool correct_ac = false;

    LOG_WRITE("z - 1 = %d, dc = %f", z - 1, CalculateDC(z - 1));
    LOG_WRITE("z = %d, ac = %e, dc = %e", z, Calculator::GetAC(&correct_ac), (double)dc);
    LOG_WRITE("z + 1 = %d, dc = %f", z + 1, CalculateDC(z + 1));

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


double Calibrator::CalibratorZero::CalculateDC(int zero)
{
    cal.zero[range].SetConst(zero);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    return Calculator::GetDC(&correct_dc);
}


bool Calibrator::CalibrateGain(int range)
{
    cal.gain[range].Set(1.0);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    double dc = std::fabs(Calculator::GetDC(&correct_dc));

    double k = Range::Max(range) / dc;

    if (range < 3)
    {
        k *= 1e3;
    }

    cal.gain[range].Set(k);

    LOG_WRITE("range = %d, dc = %f, k = %f", range, dc, k);

    return true;
}


void Calibrator::PressButtonSave()
{
    cal.Save();
}
