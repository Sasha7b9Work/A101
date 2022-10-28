// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/Measurer.h"
#include "Menu/Menu.h"


int main()
{
    HAL::Init();

    Measurer::Init();

    Menu::Init();

    Display::Init();

    while (1)
    {
        DEBUG_POINT_0;

        Measurer::Update();

        DEBUG_POINT_0;

        Display::Update();

        DEBUG_POINT_0;

        DInterface::Update();

        DEBUG_POINT_0;
    }
}
