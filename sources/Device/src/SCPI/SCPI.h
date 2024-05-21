// 2022/11/02 15:18:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"
#include "Utils/Buffer.h"
#include "SCPI/Commands.h"


namespace SCPI
{
    class InBuffer : public Buffer1024
    {
    public:
        InBuffer(Direction::E _dir) : dir(_dir) {}
        void Update();
    private:
        Command *ParseCommand(Buffer<1024> &);
        String<> FirstWord(Buffer<1024> &);
        Command *ExtractCommand();
        const Direction::E dir;
    };

    void CallbackOnReceive(Direction::E, uint8);

    void Update();
}
