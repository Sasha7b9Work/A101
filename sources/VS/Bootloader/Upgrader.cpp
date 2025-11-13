// 2025/11/12 21:48:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Upgrader.h"
#include "Utils/Timer.h"
#include "Communicator/ComPort.h"
#include "Frame.h"
#include "File.h"


namespace Upgrader
{
    static wxArrayString commands;          // Здесь будут храниться принятые команды
    static wxString current_command;        // Сюда принимаем текущую команду

    static TimerMS timer;

    static bool is_A101 = false;            // Если true, то мы подключены к нашему прибору

    static void AppendNewSymbol(char);

    static void RunCommand(const wxString &);
}


void Upgrader::Update()
{
    if (!ComPort::IsConnected())
    {
        return;
    }

    if (timer.ElapsedTime() > 3000 && !is_A101)
    {
        Frame::self->Disconnect();

        return;
    }

    if (commands.GetCount())
    {
        wxString command = commands[0];

        RunCommand(command);

        commands.RemoveAt(0);
    }
}


void Upgrader::Reset()
{
    timer.Reset();
    commands.Clear();
    current_command.Clear();
    is_A101 = false;

    ComPort::SendCommand("*idn?");
}


void Upgrader::OnEventReceive(uint8 *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        char symbol = (char)(*data);

        AppendNewSymbol(symbol);

        data++;
    }
}


void Upgrader::AppendNewSymbol(char symbol)
{
    if (symbol == 0x0a)
    {
        return;
    }

    if (symbol == 0x0d)
    {
        commands.push_back(current_command);

        current_command.Clear();
    }
    else
    {
        current_command.Append(symbol);
    }
}


void Upgrader::RunCommand(const wxString &command)
{
    if (command.StartsWith("OAO MNIPI"))            // Принятая строка начинается с A101 - это прибор А101
    {
        is_A101 = true;

        ComPort::SendCommand(wxString::Format("UPGRADE %08X %08X %08X", File::GetVersion(), File::GetSize(), File::GetCRC32()));
    }
    else if(command.StartsWith("UPGRADE "))         // Получен запрос на отправку новой порции данных
    {

    }
}
