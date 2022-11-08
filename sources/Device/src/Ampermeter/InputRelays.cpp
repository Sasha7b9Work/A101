// 2022/10/31 12:42:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL_PIO.h"


namespace InputRelays
{
    static bool enabled_zero = false;
    static int range = 3;
}


void InputRelays::SetRange(int _range)
{
    range = _range;

    static int states[6][7] =
    {
        {1, 0, 0, 0, 1, 1, 0},      // 2mA
        {1, 0, 1, 1, 0, 1, 0},      // 20mA
        {1, 1, 0, 1, 1, 0, 0},      // 200mA
        {0, 0, 0, 1, 0, 0, 0},      // 2A
        {1, 0, 1, 1, 0, 1, 0},      // 20A
        {1, 1, 0, 1, 1, 0, 0},      // 50A
    };

    HAL_PIO::Write(PIN_US1, states[range][0] == 1); //-V525
    HAL_PIO::Write(PIN_US2, states[range][1] == 1);
    HAL_PIO::Write(PIN_US3, states[range][2] == 1);
    HAL_PIO::Write(PIN_US4, states[range][3] == 1);

    if (enabled_zero)
    {
        HAL_PIO::Write(PIN_US6, false);
        HAL_PIO::Write(PIN_US7, false);
        HAL_PIO::Write(PIN_US8, true);
    }
    else
    {
        HAL_PIO::Write(PIN_US6, states[range][4] == 1);
        HAL_PIO::Write(PIN_US7, states[range][5] == 1);
        HAL_PIO::Write(PIN_US8, states[range][6] == 1);
    }
}


void InputRelays::DisableZero()
{
    enabled_zero = false;
    SetRange(range);
}


void InputRelays::EnableZero()
{
    enabled_zero = true;
    SetRange(range);
}


bool InputRelays::IsEnabledZero()
{
    return enabled_zero;
}


int InputRelays::GetRange()
{
    return range;
}
