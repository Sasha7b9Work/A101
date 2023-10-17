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
        float CalculateDC(int zero);
    };

    static void (*callbackUpdate)() = nullptr;

    // ќткалибровать усиление
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
    zero.SetVar(AD7691::GetAverageValue());
    InputRelays::DisableZero();

    zero.SetConst(const_val);

    float dc = CalculateDC(0);

    float dc1000 = CalculateDC(1000);

    int z = 0;

    int delta = (dc1000 > dc) ? 1000 : -1000;                              // Ќа эту величину будем увеличивать ноль в каждой итерации

    {
        for (int i = 0; i < 4; i++)
        {
            float sign = Math::Sign(dc);

            while (std::fabsf(sign - Math::Sign(dc)) < 1e-3f)
            {
                callbackUpdate();

                dc = CalculateDC(z);

                if (std::fabsf(dc) < 1e-10f)
                {
                    break;
                }

                z += delta;
            }

            delta = -delta / 10;
        }
    }

    bool correct_ac = false;

    LOG_WRITE("z = %d, ac = %e, dc = %e", z, (double)Calculator::GetAC(&correct_ac), (double)dc);

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


float Calibrator::CalibratorZero::CalculateDC(int zero)
{
    cal.zero[range].SetConst(zero);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    return Calculator::GetDC(&correct_dc);
}


bool Calibrator::CalibrateGain(int range)
{
    cal.gain[range].Set(1.0f);

    Ampermeter::MeasurementCycle();
    Calculator::AppendData();

    bool correct_dc = false;

    float dc = std::fabsf(Calculator::GetDC(&correct_dc));

    float k = Range::Max(range) / dc;

    if (range < 3)
    {
        k *= 1e3f;
    }

    cal.gain[range].Set(k);

    LOG_WRITE("range = %d, dc = %f, k = %f", range, (double)dc, (double)k);

    return true;
}


void Calibrator::PressButtonSave()
{
    cal.Save();
}
