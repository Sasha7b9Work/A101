// 2022/11/01 12:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calibrator.h"
#include "Nextion/Nextion.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include "Settings.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/InputRelays.h"
#include "Utils/Math.h"
#include <cstdio>
#include <cmath>


namespace Calibrator
{
    struct TimeLine
    {
        void Reset();
        void Draw();
    private:
        static const int x = 150;
        static const int y = 220;
        static const int width = 500;
        static const int height = 50;

        int last = 0;                   // Ïîñëåäíÿÿ çàêğàøåííàÿ ëèíèÿ

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

    static TimeLine timeLine;

    static Settings stored_set;

    static bool event_skip = false;
    static bool event_ready = false;

    static bool in_process = false;

    // level - 0: 0mA, 1 - ×ÅÒÈÎÉÊ ÕÒÏ×ÅÎØ
    static void DrawPromt(int range, int level);

    static void ProcedureCalibrate(int range, int level);

    static void CalibrateHardware(int range, int level);

    // Îòêàëèáğîâàòü óñèëåíèå
    static void CalibrateGain(int range);

    static void RestoreSettings();
}


bool Calibrator::InProcess()
{
    return in_process;
}


void Calibrator::ExecuteCalibration()
{
    in_process = true;

    stored_set = set;

    set.firLPF = false;
    set.middle_of_3 = false;
    set.smooth = false;

    Nextion::Page::Enable(1);

    for (int range = 0; range < 6; range++)
    {
        for (int level = 0; level < 2; level++)
        {
            ProcedureCalibrate(range, level);
        }
    }

    RestoreSettings();

    Nextion::FillRect(1, 1, 795, 350, Color::Background);

    static const pchar units[6] = {"2 mA", "20 mA", "200 mA", "2 A", "20 A", "50 A"};

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
        std::sprintf(buffer, "%d", set.cal.GetZero(range));

        Nextion::DrawString(x0 + width, y, width, height, 2, Color::White, Color::Background, buffer);

        std::sprintf(buffer, "%.10f", (double)set.cal.GetGainK(range));

        Nextion::DrawString(x0 + width * 2, y, width * 2, height, 2, Color::White, Color::Background, buffer);
    }

    event_ready = false;
    event_skip = false;

    while (!event_ready && !event_skip)
    {
        Nextion::Update();
    }

    Nextion::Page::Enable(0);

    PageTwo::self->SetAsCurrent();

    Indicator::OnEvent::CnageRange();

    in_process = false;
}


void Calibrator::RestoreSettings()
{
    CalibrationSettings cal_set = set.cal;

    set = stored_set;

    set.cal = cal_set;
}


void Calibrator::ProcedureCalibrate(int range, int level)
{
    DrawPromt(range, level);

    event_ready = false;
    event_skip = false;

    while (!event_ready && !event_skip)
    {
        Nextion::Update();
    }

    if (event_ready)
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
    const int zero = set.cal.GetZero(range);

    InputRelays::Range::Set(range);

    float dc = CalculateDC(0);

    float dc1000 = CalculateDC(1000);

    int z = 0;

    int delta = (dc1000 > dc) ? 1000 : -1000;                              // Íà ıòó âåëè÷èíó áóäåì óâåëè÷èâàòü íîëü â êàæäîé èòåğàöèè

    {
        for (int i = 0; i < 4; i++)
        {
            float sign = Math::Sign(dc);

            while (sign == Math::Sign(dc))
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
        set.cal.SetZero(range, z);
    }
    else
    {
        set.cal.SetZero(range, zero);
    }
}


float Calibrator::CalibratorZero::CalculateDC(int zero)
{
    set.cal.SetZero(range, zero);
    Ampermeter::ReadData();
    Calculator::AppendData();
    return Calculator::GetDC();
}


void Calibrator::CalibrateGain(int)
{
//    set.cal.SetGain(range, 1.0f);
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


void Calibrator::OnEvent::ButtonReady()
{
    event_ready = true;
}


void Calibrator::OnEvent::ButtonSkip()
{
    event_skip = true;
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

    std::sprintf(buffer, "ëÁÌÉÂÒÏ×ËÁ ÄÉÁĞÁÚÏÎÁ %s.", ranges[range]);

    Nextion::DrawString(10, y, 780, height, 2, Color::White, Color::Background, buffer, 1);

    y += delta + delta / 2;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "ğÏÄÁÊÔÅ ÎÁ ×ÈÏÄ ÁÍĞÅÒÍÅÔÒÁ");

    y += delta;

    std::sprintf(buffer, "ĞÏÓÔÏÑÎÎÙÊ ÔÏË ×ÅÌÉŞÉÎÏÊ %s", level == 0 ? "0 mA" : ranges[range]);

    Nextion::DrawString(x, y, width + 20, height, 2, Color::White, Color::Background, buffer);

    y += delta;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "É ÎÁÖÍÉÔÅ ËÎÏĞËÕ <çÏÔÏ×Ï>.");
}
