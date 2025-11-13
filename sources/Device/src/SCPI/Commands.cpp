// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "defines.cpp"
#include "SCPI/Commands.h"
#include "Device.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/AD7691.h"
#include "SCPI/Bootloader.h"


namespace SCPI
{
    void Send(Direction::E, pchar);

    void Error(Direction::E, pchar);
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

    if (set.meas_on_display.IsDC() || set.meas_on_display.IsAC_DC())
    {
        message.Append("I=");
        message.Append(PageMain::wndDC.GetSign());
        message.Append(PageMain::wndDC.GetDigits());
        message.Append(" ");
        message.Append(PageMain::wndDC.GetUnits(Lang::EN));
    }

    if (set.meas_on_display.IsAC_DC())
    {
        message.Append(";");
    }

    if (set.meas_on_display.IsAC() || set.meas_on_display.IsAC_DC())
    {
        message.Append("J=");
        message.Append(PageMain::wndAC.GetDigits());
        message.Append(" ");
        message.Append(PageMain::wndAC.GetUnits(Lang::EN));
    }

    Send(dir, message.c_str());

    return true;
}


bool SCPI::CommandRangeRequest::Execute(Direction::E dir)
{
    static const pchar labels[MeasuresOnDisplay::Count] =
    {
        "IJ", "I", "J"
    };

    String<> message("%s%d", labels[set.meas_on_display.Current()], Range::Current());

    Send(dir, message.c_str());

    return true;
}


bool SCPI::CommandRangeFrequencyRequest::Execute(Direction::E dir)
{
    static const pchar ranges[SampleRate::Count] =
    {
        "D0", "D1", "D2"
    };

    Send(dir, ranges[SampleRate::Get()]);

    return true;
}


bool SCPI::CommandRangeI::Execute(Direction::E)
{
    PageMain::SetMeasuresOnDisplay(MeasuresOnDisplay::DC);
    PageMain::SetRange(range);

    return true;
}


bool SCPI::CommandRangeFrequency::Execute(Direction::E)
{
    PageMain::SetSampleRate(range);

    return true;
}


bool SCPI::CommandRangeJ::Execute(Direction::E)
{
    PageMain::SetMeasuresOnDisplay(MeasuresOnDisplay::AC);
    PageMain::SetRange(range);

    return true;
}


bool SCPI::CommandRangeIJ::Execute(Direction::E)
{
    PageMain::SetMeasuresOnDisplay(MeasuresOnDisplay::AC_DC);
    PageMain::SetRange(range);

    return true;
}


bool SCPI::CommandZero::Execute(Direction::E)
{
    PageMain::EnableZero(type == 'I' ? MeasuresOnDisplay::DC : MeasuresOnDisplay::AC, enabled);

    return true;
}


bool SCPI::CommandINFO::Execute(Direction::E dir)
{
    String<128> message;

    message.AppendFormat("VERSION=%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
    message.AppendFormat("DATE BUILD=%s\n", DATE_BUILD);
    message.AppendFormat("SCPI=%s", VERSION_SCPI);

    Send(dir, message.c_str());

    return true;
}


bool SCPI::CommandUpgradeFirmware::Execute(Direction::E)
{
    Bootloader::Run(version_build_new, size, crc32);

    return true;
}
