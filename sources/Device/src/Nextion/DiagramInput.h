// 2022/10/19 09:03:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace DiagramInput
{
    void InstallData(int data[1024]);

    void _InstallData();

    void Draw();

    void Reset();

    // Если true, то не надо обновлять амперметр, чьё обновление занимает порядком времени - более 300 мс
    bool DataInstalled();
}
