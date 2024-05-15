// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/Commands.h"
#include "Device.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


namespace SCPI
{
    void Send(Direction::E, pchar);

    void Error(Direction::E, pchar);
}


bool SCPI::Command::Execute(Direction::E)
{
    return false;
}


bool SCPI::CommandWithParameters::Execute(Direction::E)
{
    return false;
}


bool SCPI::CommandIDN::Execute(Direction::E dir)
{
    String<> message("OAO MNIPI, A101, %04u %04u, v1.0.1", set.serial_number & 0xFFFF, (set.serial_number >> 16));

    Send(dir, message.c_str());
    return true;
}


bool SCPI::CommandMEAS::Execute(Direction::E dir)
{
    String<> message;

    if (MeasuresOnDisplay::IsDC() || MeasuresOnDisplay::IsAC_DC())
    {
        message.Append("I=");
        message.Append(PageMain::wndDC.GetControlSign().LastLabel());
        message.Append(PageMain::wndDC.GetControlDigits().LastLabel());
        message.Append(" ");
        message.Append(PageMain::wndDC.GetControlUnits().LastLabel());
    }

    if (MeasuresOnDisplay::IsAC_DC())
    {
        message.Append(";");
    }

    if (MeasuresOnDisplay::IsAC() || MeasuresOnDisplay::IsAC_DC())
    {
        message.Append("J=");
        message.Append(PageMain::wndAC.GetControlDigits().LastLabel());
        message.Append(" ");
        message.Append(PageMain::wndAC.GetControlUnits().LastLabel());
    }

    Send(dir, message.c_str());

    return true;
}


bool SCPI::CommandRangeI::Execute(Direction::E)
{
    PageMain::SetRange(MeasuresOnDisplay::DC, range);

    return true;
}


bool SCPI::CommandRangeJ::Execute(Direction::E)
{
    PageMain::SetRange(MeasuresOnDisplay::AC, range);

    return true;
}


bool SCPI::CommandRangeIJ::Execute(Direction::E)
{
    PageMain::SetRange(MeasuresOnDisplay::AC_DC, range);

    return true;
}


bool SCPI::CommandZero::Execute(Direction::E)
{
    PageMain::EnableZero(type == 'I' ? MeasuresOnDisplay::DC : MeasuresOnDisplay::AC, enabled);

    return true;
}
