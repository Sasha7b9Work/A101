// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer.h"
#include "Hardware/Communicator.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Ampermeter/AD7691.h"


namespace SCPI
{
    struct Command
    {
        virtual bool Execute(Direction::E) = 0;
        virtual ~Command() { }
    };


    struct CommandNull : public Command
    {
        virtual bool Execute(Direction::E) override { return false; }
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


    struct CommandINFO : public Command
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

    struct CommandRangeRequest : public Command
    {
        CommandRangeRequest() { }
        virtual bool Execute(Direction::E) override;
    };

    struct CommandRangeFrequency : public Command
    {
        CommandRangeFrequency(int _range) : range((SampleRate::E)_range) { }
        virtual bool Execute(Direction::E) override;
    protected:
        SampleRate::E range;
    };

    struct CommandRangeFrequencyRequest : public Command
    {
        CommandRangeFrequencyRequest() { }
        virtual bool Execute(Direction::E) override;
    };

    struct CommandZero : public Command
    {
        CommandZero(char _type, bool _enabled) : type(_type), enabled(_enabled) { }
        virtual bool Execute(Direction::E) override;
    protected:
        char type;
        bool enabled;
    };

    struct CommandUpgradeFirmware : public Command
    {
        CommandUpgradeFirmware(uint version_new, uint _size, uint _crc32) : version_build_new(version_new), size(_size), crc32(_crc32) { }
        virtual bool Execute(Direction::E) override;
    protected:
        uint version_build_new = 0;
        uint size = 0;
        uint crc32 = 0;
    };
}
