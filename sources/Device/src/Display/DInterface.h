// 2022/10/17 15:30:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ReturnCodeDI
{
    enum E
    {
        InvalidInstruction = 0x00,
        InstructionSuccessful = 0x01,
        InvalidComponentID = 0x02,
        InvalidPageID = 0x03,
        InvalidPictureID = 0x04,
        InvalidFontID = 0x05,
        InvalidFileOperation = 0x06,
        InvalidCRC = 0x09,
        InvalidBaudRate = 0x11,
        InvalidWaveformIDorChannel = 0x12,
        InvalidVariableNameOrAttribute = 0x1A,
        InvalidVariableOperation = 0x1B,
        AssignmentFailedToAssign = 0x1C,
        EEPROMoperationFailed = 0x1D,
        InvalidQuantityOfParameters = 0x1E,
        IOoperationFailed = 0x1F,
        EscapeCharacterInvalid = 0x20,
        VariableNameTooLong = 0x23,
        SerialBufferOverflow = 0x24,
        NextionReady = 0x88,
        StartMicroSDupgrade = 0x89,
        TransparentDataFinished = 0xFD,
        TransparentDataReady = 0xFE,
        _None = 0xFF
    };
};


namespace Display
{
    namespace Interface
    {
        void SendCommand(pchar);

        void SendByte(uint8);

        void SendCommandFormat(pchar, ...);

        void CallbackOnReceive(uint8);

        ReturnCodeDI::E LastCode();
    }
}
