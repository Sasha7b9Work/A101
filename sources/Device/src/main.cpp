// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/Ampermeter.h"
#include "Menu/Menu.h"
#include "Utils/Profiler.h"
#include "Generator/Generator.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"


int main()
{
    HAL::Init();

    HAL_TIM::Delay(500);

    Ampermeter::Init();

    Menu::Init();

    Display::Init();

    Generator::SetPicAC(1);

    Generator::SetFrequency(1024.0);

    Generator::Disable();

    while (1)
    {
        TimeMeterMS meter;

        Ampermeter::Update();

        Display::Update();

        Nextion::Update();

        Profiler::Update();

        SCPI::Update();

        LOG_WRITE("time frame %d ms", meter.ElapsedTime());
    }
}
