// 2022/10/28 11:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"


/*
*  Сюда выводится измерения
*/


namespace Indicator
{
    void Init();

    void Update();

    // Вызов этой функции устанавливает размер автоматически в зависимости от установленных режимов
    void AutoSize();

    void SetMeasures(float dc, float ac);

    // Установить признак переполнения
    void SetOverflow();

    // Пиковое значение АЦП
    void SetStatisticsADC(int peak, int ave, int min, int max);

    namespace OnEvent
    {
        void CnageRange();

        // Послать num измерений в Communicator
        void SendDataToCommunicator(Direction::E, int num);
    }
}
