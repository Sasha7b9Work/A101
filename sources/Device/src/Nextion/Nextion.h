// 2022/10/17 15:30:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Nextion/Colors.h"


// Код ответа дисплея
struct ResponseCode
{
    enum E
    {
        InvalidInstruction             = 0x00,
        InstructionSuccessful          = 0x01,
        InvalidComponentID             = 0x02,
        InvalidPageID                  = 0x03,
        InvalidPictureID               = 0x04,
        InvalidFontID                  = 0x05,
        InvalidFileOperation           = 0x06,
        InvalidCRC                     = 0x09,
        InvalidBaudRate                = 0x11,
        InvalidWaveformIDorChannel     = 0x12,
        InvalidVariableNameOrAttribute = 0x1A,
        InvalidVariableOperation       = 0x1B,
        AssignmentFailedToAssign       = 0x1C,
        EEPROMoperationFailed          = 0x1D,
        InvalidQuantityOfParameters    = 0x1E,
        IOoperationFailed              = 0x1F,
        EscapeCharacterInvalid         = 0x20,
        VariableNameTooLong            = 0x23,
        SerialBufferOverflow           = 0x24,
        NextionReady                   = 0x88,
        StartMicroSDupgrade            = 0x89,
        TransparentDataFinished        = 0xFD,
        TransparentDataReady           = 0xFE,
        None                           = 0xFF
    };
};


namespace Nextion
{
    // Функция вызывается в главном цикле
    void Update();

    // Эта функция вызвается по приходу байта с дисплея
    void CallbackOnReceive(char);

    namespace LastCode
    {
        void Set(ResponseCode::E);
        ResponseCode::E Get();
    }

    void DrawRect(int x, int y, int width, int height, const Color & = Color::Count);

    void FillRect(int x, int y, int width, int height, const Color & = Color::Count);

    void DrawLine(int x1, int y1, int x2, int y2, const Color & = Color::Count);

    void DrawLineH(int y, int x1, int x2, const Color & = Color::Count);

    void DrawLineV(int x, int y1, int y2, const Color & = Color::Count);

    // h_align - горизонтальное выравнивание. 0 - лево, 1 - центр
    void DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar, int h_align = 0, int v_align = 0);

    void SendCommandFormat(const char *, ...);

    void SetVisible(pchar name, bool);

    void SetValue(pchar name, int);

    namespace Page
    {
        // 0 - основная страница
        // 1 - страница калибровки
        void Enable(int number);
    }

    namespace WaveInput
    {
        void Draw(uint8 *points, int num_points);

        // 0 - маленький, 1 - большой
        void Enable(int size);

        // 0 - маленький, 1 - большой
        void Disable(int size);
    }

    namespace Text
    {
        void SetLabel(pchar name, pchar label);

        void SetVisible(pchar name, bool);
    }

    namespace WaveFFT
    {
        // 0 - маленький, 1 - большой
        void Enable(int size);

        // 0 - маленький, 1 - большой
        void Disable(int size);
    }

    namespace Button
    {
        void SetText(pchar name_button, pchar text);

        void Highligth(pchar name_button, bool);

        void Enable(pchar name_button);

        void Disable(pchar name_button);
    }
}
