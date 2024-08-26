// 2024/08/22 16:41:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Keyboard
{
    int CurrentX();

    int CurrentY();

    void CallbackOnPress(int, int);

    void CallbackOnRelease(int, int);
}
