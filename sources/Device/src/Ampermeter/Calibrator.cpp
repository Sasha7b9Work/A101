// 2022/11/01 12:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calibrator.h"
#include "Nextion/Nextion.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"


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

    Nextion::DrawString(10, 10, 780, 300, 2,
        Color::White, Color::Background,
        "Ìâíê÷ôðþìâ äêâòâÿðïâ 50 Â.\n"
        "ðÏÄÁÊÔÅ ÎÁ ×ÈÏÄ ÁÍÐÅÒÍÅÔÒÁ\n"
    );

    HAL_TIM::Delay(1000);

    Nextion::Page::Enable(0);

    PageTwo::self->SetAsCurrent();

    Indicator::OnCnageRangeEvent();
}


bool Calibrator::InProcess()
{
    return in_process;
}
