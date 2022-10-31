// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace InputRelays
{
    void SetRange(int);

    // Включить режим "нуль" (измерения невозможны)
    void EnableZero();

    // Выключить режим "нуль" (измерения невозможны)
    void DisableZero();
}
