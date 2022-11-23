// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"
#include "Hardware/Communicator.h"


namespace SCPI
{

    class Command
    {
    public:
        virtual bool Execute();
        virtual ~Command() {}
    };


    class CommandIDN : public Command
    {
    public:
        virtual bool Execute() override;
        virtual ~CommandIDN() override {}
    };

}
