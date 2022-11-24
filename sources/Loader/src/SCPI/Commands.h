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
        virtual bool Execute(Direction::E);
        virtual ~Command() {}
    };


    class CommandWithParameters : public Command
    {
    public:
        CommandWithParameters(pchar _params) { params.SetFormat(_params); }
        virtual ~CommandWithParameters() override {}
        virtual bool Execute(Direction::E) override;
    protected:
        String<> params;
    };


    class CommandIDN : public Command
    {
    public:
        virtual bool Execute(Direction::E) override;
        virtual ~CommandIDN() override {}
    };


    class CommandRST : public Command
    {
    public:
        virtual bool Execute(Direction::E) override;
        virtual ~CommandRST() override {}
    };


    class CommandRANGE : public CommandWithParameters
    {
    public:
        CommandRANGE(pchar par) : CommandWithParameters(par) {}
        virtual ~CommandRANGE() override {}
        virtual bool Execute(Direction::E) override;
    };


    class CommandDATA : public CommandWithParameters
    {
    public:
        CommandDATA(pchar par) : CommandWithParameters(par) {}
        virtual ~CommandDATA() override {}
        virtual bool Execute(Direction::E) override;
    };
}
