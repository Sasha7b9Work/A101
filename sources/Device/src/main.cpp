// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Display.h"


int main()
{
    HAL::Init();

    float voltageDC = 0.0f;

    while (1)
    {
        voltageDC += 1.0f;

        Display::SetDC(voltageDC);
    }
}
