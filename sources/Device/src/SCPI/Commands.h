// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"
#include "Hardware/Communicator.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"


namespace SCPI
{
    struct Command
    {
        virtual bool Execute(Direction::E);
        virtual ~Command() {}
    };


    struct CommandWithParameters : public Command
    {
        CommandWithParameters(pchar _params) { params.SetFormat(_params); }
        virtual ~CommandWithParameters() override {}
        virtual bool Execute(Direction::E) override;
    protected:
        String<> params;
    };


    struct CommandIDN : public Command
    {
        virtual bool Execute(Direction::E) override;
        virtual ~CommandIDN() override {}
    };
}
