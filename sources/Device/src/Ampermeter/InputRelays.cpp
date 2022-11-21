// 2022/10/31 12:42:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Display/Indicator.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Ampermeter/Calibrator.h"
#include "Settings.h"


namespace Range
{
    static int current = 3;
    static int prev = 0;

    static void Load()
    {
        Set(current);
    }
}


void Range::Set(int _range)
{
    bool need_event = (current != _range);

    prev = current;
    current = _range;

    if (need_event)
    {
        if (!Calibrator::InProcess())
        {
            Indicator::OnEvent::CnageRange();
        }
    }

    static int states[6][7] =
    {
        {1, 0, 0, 0, 1, 1, 0},      // 2mA
        {1, 0, 1, 1, 0, 1, 0},      // 20mA
        {1, 1, 0, 1, 1, 0, 0},      // 200mA
        {0, 0, 0, 1, 0, 0, 0},      // 2A
        {1, 0, 1, 1, 1, 0, 0},      // 20A
        {1, 1, 0, 1, 0, 1, 0}       // 50A
    };

    HAL_PIO::Write(PIN_US1, states[current][0] == 1); //-V525
    HAL_PIO::Write(PIN_US2, states[current][1] == 1);
    HAL_PIO::Write(PIN_US3, states[current][2] == 1);
    HAL_PIO::Write(PIN_US4, states[current][3] == 1);

    if (set.enabled_zero)
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
}


void InputRelays::DisableZero()
{
    set.enabled_zero = false;
    Range::Load();
}


void InputRelays::EnableZero()
{
    set.enabled_zero = true;
    Range::Load();
}


bool InputRelays::IsEnabledZero()
{
    return set.enabled_zero;
}


int Range::Current()
{
    return current;
}


int Range::Prev()
{
    return prev;
}
