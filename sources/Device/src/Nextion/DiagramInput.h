// 2022/10/19 09:03:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace DiagramInput
{
    void InstallData();

    // Это вспомогательная функция. Можно использовать при отладке
    void InstallData2(float data[1024]);

    void Draw();

    void Reset(bool clear);

    // Если true, то не надо обновлять амперметр, чьё обновление занимает порядком времени - более 300 мс
    bool DataInstalled();
}
