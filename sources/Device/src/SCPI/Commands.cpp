// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/Commands.h"
#include "Device.h"
#include "Menu/Pages/Pages.h"


namespace SCPI
{
    void Send(pchar);
}


bool SCPI::Command::Execute()
{
    return false;
}


bool SCPI::CommandIDN::Execute()
{
    Send("OAO MNIPI, A101");
    return true;
}


bool SCPI::CommandRST::Execute()
{
    Device::Reset();
    return true;
}


SCPI::CommandRANGE::CommandRANGE(pchar data)
{
    params.SetFormat(data);
}


bool SCPI::CommandRANGE::Execute()
{
    String<> char_range = params.GetWord(0);

    static const pchar titles[6] = { "2MA", "20MA", "200MA", "2A", "20A", "50A" };

    for (int i = 0; i < 6; i++)
    {
        if (char_range == titles[i])
        {
            PageMain::self->GetButton(i)->Press();
            break;
        }
    }

    return true;
}
