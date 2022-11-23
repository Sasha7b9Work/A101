// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"


namespace SCPI
{

    class Command : public Buffer<uint8, 128>
    {
    public:
        virtual bool Execute();
    };


    class CommandIDN : public Command
    {
    public:
        virtual bool Execute() override;
    };

}
