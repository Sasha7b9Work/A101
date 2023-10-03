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
#include <cstdio>
#include <cmath>


namespace Calibrator
{
    static bool in_progress = false;

    static uint time_start = 0;

    struct TimeLine
    {
        void Reset();
        void Draw();
    private:
        static const int x = 175;
        static const int y = 220;
        static const int width = 450;
        static const int height = 50;

        int last = 0;                   // Последняя закрашенная линия

        Color color = Color::White;
    };

    struct CalibratorZero
    {
        CalibratorZero(int _range) : range(_range) {}
        void Run();
    private:
        int range;
        float CalculateDC(int zero);
    };

    // Эта функция будет вызываться после отработки калибровки
    static void (*funcAfterRun)() = nullptr;

    static TimeLine timeLine;

    static void CalibrateHardware(int range, int level);

    // Откалибровать усиление
    static void CalibrateGain(int range);
}


void Calibrator::SetCallbackAfterRun(void (*callback)())
{
    funcAfterRun = callback;
}


void Calibrator::CalibrateHardware(int range, int level)
{
    timeLine.Reset();

    Nextion::FillRect(100, 90, 600, 200, Color::Background);

    Range::Set(range);

    TimeMeterMS().Wait(1000);

    if (level == 0)
    {
        CalibratorZero(range).Run();
    }
    else if (level == 1)
    {
        CalibrateGain(range);
    }

    Nextion::FillRect(100, 90, 600, 200, Color::Background);
}


void Calibrator::CalibratorZero::Run()
{
    const int zero = cal.GetZero(range);

    float dc = CalculateDC(0);

    float dc1000 = CalculateDC(1000);

    int z = 0;

    int delta = (dc1000 > dc) ? 1000 : -1000;                              // На эту величину будем увеличивать ноль в каждой итерации

    {
        for (int i = 0; i < 4; i++)
        {
            float sign = Math::Sign(dc);

            while (std::fabsf(sign - Math::Sign(dc)) < 1e-3f)
            {
                timeLine.Draw();

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

    LOG_WRITE("z = %d, ac = %e, dc = %e", z, (double)Calculator::GetAC(), (double)dc);

    if (Math::Abs(z) < 10000)
    {
        cal.SetZero(range, z);
    }
    else
    {
        cal.SetZero(range, zero);
    }
}


float Calibrator::CalibratorZero::CalculateDC(int zero)
{
    cal.SetZero(range, zero);

    Ampermeter::ReadData();
    Calculator::AppendData();

    return Calculator::GetDC();
}


void Calibrator::CalibrateGain(int range)
{
    cal.SetGainK(range, 1.0f);

    Ampermeter::ReadData();
    Calculator::AppendData();

    float dc = std::fabsf(Calculator::GetDC());

    float k = Range::Max(range) / dc;

    if (range < 3)
    {
        k *= 1e3f;
    }

    cal.SetGainK(range, k);

    LOG_WRITE("range = %d, dc = %f, k = %f", range, (double)dc, (double)k);
}


void Calibrator::TimeLine::Reset()
{
    Nextion::DrawRect(x, y, width, height, Color::White);

    last = x;

    color = Color::White;
}


void Calibrator::TimeLine::Draw()
{
    last++;

    Nextion::DrawLineV(last, y + 1, y + height - 1, color);

    if (last == x + width - 1)
    {
        last = x;

        color = (color.value == Color::White.value) ? Color::Background : Color::White;
    }
}


void Calibrator::Update()
{
    if (in_progress)
    {
        if (TIME_MS >= time_start + 5000)
        {
            in_progress = false;

            if (funcAfterRun)
            {
                funcAfterRun();
            }
        }
    }
}


void Calibrator::PressButtonRun()
{
    in_progress = true;

    time_start = TIME_MS;
}


void Calibrator::PressButtonSave()
{

}
