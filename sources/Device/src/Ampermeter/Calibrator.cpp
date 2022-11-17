// 2022/11/01 12:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calibrator.h"
#include "Nextion/Nextion.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include "Display/Indicator.h"


namespace Calibrator
{
    static bool in_process = false;

    enum class State
    {
        Start
    };

    static State state = State::Start;
}


void Calibrator::ExecuteCalibration()
{
    in_process = true;

    Nextion::Page::Enable(1);

    TimeMeterMS meter;

    

    Nextion::Page::Enable(0);

    PageTwo::self->SetAsCurrent();

    Indicator::OnCnageRangeEvent();
}


bool Calibrator::InProcess()
{
    return in_process;
}
