// 2022/10/31 12:42:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Nextion.h"
#include <cstdio>


int Range::current = 3;
int Range::prev = 0;


namespace InputRelays
{
    static bool zero_enabled = false;
}


REAL Range::Max(int range)
{
    static const REAL max[6] = { 2.0, 2e1, 2e2, 2e3, 2e4, 5e4 };

    return max[range];
}


void Range::Set(int _range, bool reset_measures)
{
    const bool disable_zero = current != _range;        // Если не переключаем на другой диапазон - выключать зеро не надо

    PageCalibration::OnEventChangeRange();

    prev = current;
    current = _range;

    static int states[6][7] =
    {
     //                 A0  A1  A2
     // US1 US2 US3 US4 US6 US7 US8
        {1,  0,  0,  0,  1,  1,  0},      // 2mA
        {1,  0,  1,  1,  0,  1,  0},      // 20mA
        {1,  1,  0,  1,  1,  0,  0},      // 200mA
        {0,  0,  0,  1,  0,  0,  0},      // 2A
        {1,  1,  0,  1,  0,  1,  0},      // 20A
        {1,  0,  1,  1,  1,  0,  0}       // 50A
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

    if (reset_measures)
    {
        PageMain::OnEventChangeRange();
        Ampermeter::OnEventChangeRange();
    }

    if (Ampermeter::AVP::IsEnabled())
    {
        for (int i = 0; i < 6; i++)
        {
            char name[32] = "\0";

            std::sprintf(name, "t1%d", i + 1);
            Nextion::Text::SetVisible(name, _range == i);

//            std::sprintf(name, "bt%d", i);
//            Nextion::Button::SetValue(name, _range == i ? 1 : 0);
        }
    }

    if (disable_zero)
    {
        Ampermeter::Set::ZeroAC::_Disable();
        Ampermeter::Set::ZeroDC::_Disable();
    }
}


void InputRelays::DisableZero(bool reset_measures)
{
    zero_enabled = false;

    Range::Load(reset_measures);
}


void InputRelays::EnableZero(bool reset_measures)
{
    zero_enabled = true;

    Range::Load(reset_measures);
}


bool InputRelays::ZeroIsEanbled()
{
    return zero_enabled;
}


int Range::Current()
{
    return current;
}
