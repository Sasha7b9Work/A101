// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/Commands.h"
#include "Device.h"
#include "Updater.h"


namespace SCPI
{
    void Send(pchar);
}


bool SCPI::Command::Execute()
{
    return false;
}


bool SCPI::CommandWithParameters::Execute()
{
    return false;
}


bool SCPI::CommandREQUEST::Execute()
{
    Send("a101Y");

    return true;
}


bool SCPI::CommandSIZE::Execute()
{
    String<> char_size = params.GetWord(0);

    int size = 0;

    if (char_size.ToInt(&size))
    {
        Updater::SetSize(size);

        State::Set(State::InProcessUpdate);

        Send("?");
    }

    return true;
}
