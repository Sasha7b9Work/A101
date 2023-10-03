// 2022/11/01 12:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Calibrator
{
    void Run(int range, int level);

    void PressButtonSave();

    // Эта функция будет вызываться периодически во время калибровки
    void SetCallbackUpdate(void (*)());

    // Устанавливает функцию, вызываемую после отработки PressButtonRun()
    void SetCallbackOnComplete(void (*)());
}
