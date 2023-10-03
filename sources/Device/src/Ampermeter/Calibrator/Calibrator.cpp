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

    static bool event_run = false;

    // level - 0: 0mA, 1 - ЧЕТИОЙК ХТПЧЕОШ
    static void DrawPromt(int range, int level);

    static void ProcedureCalibrate(int range, int level);

    static void CalibrateHardware(int range, int level);

    // Откалибровать усиление
    static void CalibrateGain(int range);

    static void DrawParameters();

    void ExecuteCalibration();
}


void Calibrator::SetCallbackAfterRun(void (*callback)())
{
    funcAfterRun = callback;
}


void Calibrator::ExecuteCalibration()
{
    Settings::Storage::Store(set);
    CalibrationSettings::Storage::Store(cal);

    set.firLPF = false;
    set.middle_of_3 = false;
    set.smooth = false;
    set.enabled_zero = false;

    Nextion::Page::Enable(1);

    for (int range = 0; range < 6; range++)
    {
        for (int level = 0; level < 2; level++)
        {
            Range::Set(3);

            ProcedureCalibrate(range, level);
        }
    }

    Settings::Storage::Restore(set);

    DrawParameters();

    if (event_run)
    {
        cal.Save();
    }
    else
    {
        CalibrationSettings::Storage::Restore(cal);
    }

    Nextion::Page::Enable(0);

    PageGraph::self->SetAsCurrent();
}


void Calibrator::DrawParameters()
{
    Nextion::FillRect(1, 1, 795, 350, Color::Background);

    static const pchar units[6] = { "2 mA", "20 mA", "200 mA", "2 A", "20 A", "50 A" };

    for (int range = 0; range < 6; range++)
    {
        int width = 140;
        int height = 40;
        int x0 = 150;
        int dY = 50;
        int y0 = 30;
        int y = y0 + range * dY;

        Nextion::DrawString(x0, y, width, height, 2, Color::White, Color::Background, units[range]);

        char buffer[30];
        std::sprintf(buffer, "%d", cal.GetZero(range));

        Nextion::DrawString(x0 + width, y, width, height, 2, Color::White, Color::Background, buffer);

        std::sprintf(buffer, "%.10f", (double)cal.GetGainK(range));

        Nextion::DrawString(x0 + width * 2, y, width * 2, height, 2, Color::White, Color::Background, buffer);
    }
}


void Calibrator::ProcedureCalibrate(int range, int level)
{
    DrawPromt(range, level);

    if (event_run)
    {
        Nextion::Button::Disable("buttonOk");
        Nextion::Button::Disable("buttonCancel");

        CalibrateHardware(range, level);

        Nextion::Button::Enable("buttonOk");
        Nextion::Button::Enable("buttonCancel");
    }
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


void Calibrator::DrawPromt(int range, int level)
{
    const int height = 40;
    const int width = 550;
    const int delta = 60;
    const int x = 140;

    int y = 30;

    static const char *ranges[6] = { "2 mA", "20 mA", "200 mA", "2 A", "20 A", "50 A" };

    char buffer[50] = { '\0' };

    std::sprintf(buffer, "лБМЙВТПЧЛБ ДЙБРБЪПОБ %s.", ranges[range]);

    Nextion::DrawString(10, y, 780, height, 2, Color::White, Color::Background, buffer, 1);

    y += delta + delta / 2;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "рПДБКФЕ ОБ ЧИПД БНРЕТНЕФТБ");

    y += delta;

    std::sprintf(buffer, "РПУФПСООЩК ФПЛ ЧЕМЙЮЙОПК %s", level == 0 ? "0 mA" : ranges[range]);

    Nextion::DrawString(x, y, width + 20, height, 2, Color::White, Color::Background, buffer);

    y += delta;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "Й ОБЦНЙФЕ ЛОПРЛХ <зПФПЧП>.");
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
