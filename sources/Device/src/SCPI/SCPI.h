// 2022/11/02 15:18:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"
#include "Utils/Buffer.h"
#include "SCPI/Commands.h"


namespace SCPI
{
    class InBuffer : public Buffer2048
    {
    public:
        InBuffer(Direction::E _dir) : dir(_dir) {}
        void Update();
    private:
        Command *ParseCommand(pchar);
        String<> FirstWord(pchar);
        Command *ExtractCommand();
        const Direction::E dir;
    };

    void CallbackOnReceive(Direction::E, char);

    void Update();

    bool ExistData();
}
