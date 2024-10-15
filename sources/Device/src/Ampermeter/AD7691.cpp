// 2022/10/18 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Settings/Settings.h"
#include "Ampermeter/BufferADC.h"
#include "Utils/Math.h"
#include "stm_includes.h"
#include <cmath>


const ValueADC ValueADC::MAX = ValueADC((1 << 17) - 1);
const ValueADC ValueADC::MIN = ValueADC((1 << 17));


int ValueADC::_raw = 0;


SampleRate SampleRate::current(10);


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
}


void AD7691::Init()
{
    pinIN.Init();
    pinCLK.Init();
    pinCNV.Init();

    pinCNV.Reset();
    pinCLK.Reset();
}


#ifdef EMULATOR_ENABLED

namespace AD7691
{
    static int counter = 0;

    void ResetValue()
    {
//        counter = rand();
    }
}


static float GetSample(float freq, int num_sample)
{
    float T = 1.0f / freq;

    float samples_in_T = T / 10e-6f;

    float radians_in_T = 2.0f * M_PI;

    float radians_in_sample = radians_in_T / samples_in_T;

    return std::sinf(radians_in_sample * num_sample);
}


ValueADC AD7691::ReadValue()
{
    float amplitude = 0.5f;

    float value = amplitude * GetSample(10.123f, counter);

//    value *= (float)std::rand() / (float)RAND_MAX;

    value *= (1 << 16);

    if (value < 0)
    {
        value += (1 << 18);
    }

    counter++;

    return ValueADC((int)(value));
}

#else


void AD7691::ResetValue()
{

}


ValueADC AD7691::ReadValue()
{
    int result = 0;

    GPIOB->BSRR = GPIO_PIN_12;

    __IO uint wait = 0;   for (; wait < 40; wait++)  { }

    GPIOB->BSRR = GPIO_PIN_12 << 16;

#ifndef WIN32
        __ASM("nop");
        __ASM("nop");
#endif

    for (int i = 0; i < 18; i++)
    {
        GPIOB->BSRR = GPIO_PIN_10;

        if (GPIOC->IDR & GPIO_PIN_2)
        {
            result |= 1;
        }

#ifndef WIN32
        __ASM("nop");
        __ASM("nop");
#endif

        GPIOB->BSRR = GPIO_PIN_10 << 16;

        if (i != 17)
        {
            result <<= 1;
        }

#ifndef WIN32
        __ASM("nop");
        __ASM("nop");
#endif
    }

    return ValueADC(result);
}

#endif


ValueADC::ValueADC(int reading)
{
    _raw = reading;

    value = reading;

    if (_GET_BIT(value, 17))
    {
        value -= 1 << 18;
    }

    value -= cal.zero[Range::Current()].GetFull();
}


int AD7691::GetAverageValue()
{
    int64 sum = 0;

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        sum += AD7691::ReadValue();
    }

    return (int)((float)sum / (float)BufferADC::SIZE + 0.5f);
}
