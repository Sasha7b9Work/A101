// 2025/11/12 21:49:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Upgrader
{
    void Update();

    // Вызывается при новом подключении
    void Reset();

    void OnEventReceive(uint8 *, int);
}
