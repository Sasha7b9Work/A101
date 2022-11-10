// 2022/10/28 11:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  Сюда выводится измерения
*/


namespace Indicator
{
    void Update();

    // Вызов этой функции устанавливает размер автоматически в зависимости от установленных режимов
    void AutoSize();

    void SetMeasures(float dc, float ac, int range);

    void Reset(int range);

    void SetDeltaADC(int);
}
