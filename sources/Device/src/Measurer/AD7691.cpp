// 2022/10/18 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/AD7691.h"
#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#include <stm32f4xx_hal.h>
#ifndef WIN32
#pragma clang diagnostic pop
#endif


namespace AD7691
{
    struct Waiter
    {
        void WaitConversion()
        {
            __IO uint value = 0;
            
            for(; value < 35; value++)
            {
            }
        }

        void WaitValue()
        {
            __IO uint value = 0;
            
            for(; value < 1; value++)
            {
            }
        }
    };

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
                GPIO_SPEED_FAST,
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
                GPIO_SPEED_FAST,
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
}


void AD7691::Init()
{
    pinIN.Init();
    pinCLK.Init();
    pinCNV.Init();

    pinCNV.Reset();
    pinCLK.Reset();
}


uint AD7691::ReadValue()
{
    uint result = 0;

    GPIOB->BSRR = GPIO_PIN_12;

    Waiter waiter;

    waiter.WaitConversion();

    GPIOB->BSRR = GPIO_PIN_12 << 16;

    for (int i = 0; i < 18; i++)
    {
        GPIOB->BSRR = GPIO_PIN_10;

        result <<= 1;
        
        GPIOB->BSRR = GPIO_PIN_10 << 16;
        
        __ASM("nop");
        __ASM("nop");
        
        if (GPIOC->IDR & GPIO_PIN_2)
        {
            result |= 1U;
        }
    }

    return result;
}
