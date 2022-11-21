// 2022/11/01 12:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calibrator.h"
#include "Nextion/Nextion.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include <cstdio>


namespace Calibrator
{
    enum class State
    {
        Start
    };

//    static State state = State::Start;

    static bool event_skip = false;
    static bool event_ready = false;

    // level - 0: 0mA, 1 - верхний уровень
    static void DrawPromt(int range, int level);

    static void ProcedureCalibrate(int range, int level);

    static void CalibrateHardware(int range, int level);

    static void DrawTimeLine(uint time);
}


void Calibrator::ExecuteCalibration()
{
    Nextion::Page::Enable(1);

    for (int range = 0; range < 6; range++)
    {
        for (int level = 0; level < 2; level++)
        {
            ProcedureCalibrate(range, level);
        }
    }

    Nextion::Page::Enable(0);

    PageTwo::self->SetAsCurrent();

    Indicator::OnEvent::CnageRange();
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
        CalibrateHardware(range, level);
    }
    else if (event_skip)
    {

    }
}


void Calibrator::CalibrateHardware(int range, int level)
{
    TimeMeterMS meter;

    Nextion::FillRect(50, 50, 300, 200, Color::ButtonPress);

    while (meter.ElapsedTime() < 1000)
    {
        DrawTimeLine(meter.ElapsedTime());
    }
}


void Calibrator::DrawTimeLine(uint)
{

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

    std::sprintf(buffer, "Калибровка диапазона %s.", ranges[range]);

    Nextion::DrawString(10, y, 780, height, 2, Color::White, Color::Background, buffer, 1);

    y += delta + delta / 2;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "Подайте на вход амперметра");

    y += delta;

    std::sprintf(buffer, "постоянный ток величиной %s", level == 0 ? "0 mA" : ranges[range]);

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, buffer);

    y += delta;

    Nextion::DrawString(x, y, width, height, 2, Color::White, Color::Background, "и нажмите кнопку <Готово>.");
}
