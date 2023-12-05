// 2022/10/31 12:42:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Ampermeter.h"


int Range::current = 3;
int Range::prev = 0;


namespace InputRelays
{
    static bool zero_enabled = false;
}


REAL Range::Max(int range)
{
    static const REAL max[6] = { 2e-3, 20e-3, 200e-3, 2.0, 20.0, 50.0 };

    return max[range];
}


void Range::Set(int _range)
{
    PageCalibration::OnEventChangeRange();

    prev = current;
    current = _range;

    static int states[6][7] =
    {
        {1, 0, 0, 0, 1, 1, 0},      // 2mA
        {1, 0, 1, 1, 0, 1, 0},      // 20mA
        {1, 1, 0, 1, 1, 0, 0},      // 200mA
        {0, 0, 0, 1, 0, 0, 0},      // 2A
        {1, 1, 0, 1, 0, 1, 0},      // 20A
        {1, 0, 1, 1, 1, 0, 0}       // 50A
    };

    HAL_PIO::Write(PIN_US1, states[current][0] == 1); //-V525
    HAL_PIO::Write(PIN_US2, states[current][1] == 1);
    HAL_PIO::Write(PIN_US3, states[current][2] == 1);
    HAL_PIO::Write(PIN_US4, states[current][3] == 1);

    if (InputRelays::ZeroIsEanbled())
    {
        HAL_PIO::Write(PIN_US6, false);
        HAL_PIO::Write(PIN_US7, false);
        HAL_PIO::Write(PIN_US8, true);
    }
    else
    {
        HAL_PIO::Write(PIN_US6, states[current][4] == 1);
        HAL_PIO::Write(PIN_US7, states[current][5] == 1);
        HAL_PIO::Write(PIN_US8, states[current][6] == 1);
    }

    PageMain::OnEventChangeRange();
    Ampermeter::OnEventChangeRange();
}


void InputRelays::DisableZero()
{
    zero_enabled = false;
    Range::Load();
}


void InputRelays::EnableZero()
{
    zero_enabled = true;
    Range::Load();
}


bool InputRelays::ZeroIsEanbled()
{
    return zero_enabled;
}


int Range::Current()
{
    return current;
}
