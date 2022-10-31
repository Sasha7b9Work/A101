// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/Measurer.h"
#include "Menu/Menu.h"
#include "Utils/Profiler.h"


int main()
{
    HAL::Init();

    Measurer::Init();

    Menu::Init();

    Display::Init();

    while (1)
    {
        Measurer::Update();

        Display::Update();

        DInterface::Update();

        Profiler::Update();
    }
}
