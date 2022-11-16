// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace InputRelays
{
    namespace Range
    {
        void Set(int);

        // Текущее значение
        int Current();

        // Предыдущее значение
        int Prev();
    }

    // Включить режим "нуль" (измерения невозможны)
    void EnableZero();

    // Выключить режим "нуль" (измерения невозможны)
    void DisableZero();

    bool IsEnabledZero();
}
