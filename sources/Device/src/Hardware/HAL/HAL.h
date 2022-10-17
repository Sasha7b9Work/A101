// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
};


namespace HAL_I2C1
{
    void Init();

    void Transmit(uint8 address, uint8 data[3]);
};


namespace HAL_SPI3
{
    void Init();
    // Запускает счётчик для измерения малых отрезков времени
    void Transmit(uint8 *buffer, uint16 num);
};


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
};


namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
};
