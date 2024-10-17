// 2022/11/01 12:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Calibrator
{
    struct Type
    {
        enum E
        {
            DC,
            AC
        };
    };

    bool Run(int range, Type::E, void (*callbackUpdate)());

    bool InProgress();

    void PressButtonSave();
}
