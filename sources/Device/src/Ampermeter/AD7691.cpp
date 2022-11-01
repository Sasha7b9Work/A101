// 2022/10/18 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include "Generator/Generator.h"
#include "stm_includes.h"


namespace AD7691
{
    struct Pin
    {
        Pin(GPIO_TypeDef *_gpio, uint16 _pin) : gpio(_gpio), pin(_pin) { }
    protected:
        GPIO_TypeDef *gpio;
        uint16        pin;
    };

    struct PinIN : public Pin
    {
        PinIN(GPIO_TypeDef *_gpio, uint16 _pin) : Pin(_gpio, _pin)
        {
            Init();
        }
        void Init()
        {
            GPIO_InitTypeDef is =
            {
                pin,
                GPIO_MODE_INPUT,
                GPIO_PULLDOWN,
                GPIO_SPEED_HIGH,
                0
            };

            HAL_GPIO_Init(gpio, &is);
        }

        int Read()
        {
            return HAL_GPIO_ReadPin(gpio, pin);
        }
    };

    struct PinOUT : public Pin
    {
        PinOUT(GPIO_TypeDef *_gpio, uint16 _pin) : Pin(_gpio, _pin)
        {
            Init();
        }
        void Init()
        {
            GPIO_InitTypeDef is =
            {
                pin,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH,
                0
            };

            HAL_GPIO_Init(gpio, &is);
        }

        void Set()
        {
            HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
        }
        void Reset()
        {
            HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
        }
    };

    static PinIN pinIN(GPIOC, GPIO_PIN_2);      // 17
    static PinOUT pinCLK(GPIOB, GPIO_PIN_10);   // 47
    static PinOUT pinCNV(GPIOB, GPIO_PIN_12);   // 51

    static int ReadReal();

    static int (*funcRead)() = ReadReal;
}


void AD7691::Init()
{
    pinIN.Init();
    pinCLK.Init();
    pinCNV.Init();

    pinCNV.Reset();
    pinCLK.Reset();
}


int AD7691::ReadValue()
{
    return funcRead();
}


int AD7691::ReadReal()
{
    int result = 0;

    GPIOB->BSRR = GPIO_PIN_12;

    __IO uint wait = 0;   for (; wait < 100; wait++)  { }

    GPIOB->BSRR = GPIO_PIN_12 << 16;

    for (int i = 0; i < 18; i++)
    {
        GPIOB->BSRR = GPIO_PIN_10;

        result <<= 1;

        GPIOB->BSRR = GPIO_PIN_10 << 16;

#ifndef WIN32
        __ASM("nop");
        __ASM("nop");
        __ASM("nop");
#endif

        if (GPIOC->IDR & GPIO_PIN_2)
        {
            result |= 1U;
        }
    }

    if (_GET_BIT(result, 17))
    {
        result -= 1 << 18;
    }

    result -= 6510;

    return result;
}


void AD7691::GeneratorChangedEvent()
{
    funcRead = Generator::IsEanbled() ? Generator::ReadValue : ReadReal;
}
