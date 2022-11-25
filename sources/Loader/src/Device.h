// 2022/11/23 14:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct State
{
    enum E
    {
        WaitUpdate,             // Ождиаем обновление
        InProcessUpdate,        // В состоянии обновления
        Completed               // Завершено - можно переходить на основную программу
    };

    static void Set(E s) { current = s; }
    static E Current() { return current; }

private:

    static E current;
};


namespace Device
{
    void Init();

    void Update();
}
