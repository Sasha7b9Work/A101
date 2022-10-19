// 2022/10/18 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/ADC.h"


namespace ADC
{
    struct Waiter
    {
        void WaitConversion()
        {

        }

        void WaitValue()
        {

        }
    };

    struct Pin
    {
    };

    struct PinIN : public Pin
    {
        int Read()
        {
            return 0;
        }
    };

    struct PinOUT : public Pin
    {
        void Set() {}
        void Reset() {}
    };

    static PinIN pinIN;
    static PinOUT pinCS;
    static PinOUT pinCLK;
}


void ADC::Init()
{
    pinCS.Reset();
    pinCLK.Reset();
}


uint ADC::ReadValue()
{
    uint result = 0;

    pinCS.Set();

    Waiter waiter;

    waiter.WaitConversion();

    pinCS.Reset();

    waiter.WaitValue();

    for (int i = 0; i < 18; i++)
    {
        pinCLK.Set();

        result <<= 1;

        pinCLK.Reset();

        result |= pinIN.Read();
    }

    return result;
}
