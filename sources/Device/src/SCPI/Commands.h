// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"
#include "Hardware/Communicator.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"


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


    class CommandRST : public Command
    {
    public:
        virtual bool Execute() override;
        virtual ~CommandRST() override {}
    };


    class CommandRANGE : public Command
    {
    public:
        CommandRANGE(pchar);
        virtual ~CommandRANGE() override {}
        virtual bool Execute() override;
    private:
        String<> params;
    };
}
