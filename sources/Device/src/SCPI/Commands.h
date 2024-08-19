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
        virtual ~Command() { }
    };


    struct CommandWithParameters : public Command
    {
        CommandWithParameters(pchar _params) { params.SetFormat(_params); }
        virtual bool Execute(Direction::E) override;
    protected:
        String<> params;
    };


    struct CommandIDN : public Command
    {
        virtual bool Execute(Direction::E) override;
    };


    struct CommandMEAS : public Command
    {
        virtual bool Execute(Direction::E) override;
    };


    struct CommandRangeI : public Command
    {
        CommandRangeI(int _range) : range(_range) { }
        virtual bool Execute(Direction::E) override;
    protected:
        int range;
    };

    struct CommandRangeJ : public Command
    {
        CommandRangeJ(int _range) : range(_range) { }
        virtual bool Execute(Direction::E) override;
    protected:
        int range;
    };

    struct CommandRangeIJ : public Command
    {
        CommandRangeIJ(int _range) : range(_range) { }
        virtual bool Execute(Direction::E) override;
    protected:
        int range;
    };

    struct CommandZero : public Command
    {
        CommandZero(char _type, bool _enabled) : type(_type), enabled(_enabled) { }
        virtual bool Execute(Direction::E) override;
    protected:
        char type;
        bool enabled;
    };
}
