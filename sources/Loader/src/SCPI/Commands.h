// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"
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


    class CommandWithParameters : public Command
    {
    public:
        CommandWithParameters(pchar _params) { params.SetFormat(_params); }
        virtual ~CommandWithParameters() override {}
        virtual bool Execute() override;
    protected:
        String<> params;
    };


    class CommandREQUEST : public Command
    {
    public:
        virtual bool Execute() override;
        virtual ~CommandREQUEST() override {}
    };


    class CommandRANGE : public CommandWithParameters
    {
    public:
        CommandRANGE(pchar par) : CommandWithParameters(par) {}
        virtual ~CommandRANGE() override {}
        virtual bool Execute() override;
    };
}
