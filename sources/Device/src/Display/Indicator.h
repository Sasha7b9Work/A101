// 2022/10/28 11:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"
#include "Display/Controls/TextString.h"


/*
*  Сюда выводится измерения
*/


namespace Indicator
{
    void Update();

    // Вызов этой функции устанавливает размер автоматически в зависимости от установленных режимов
    void AutoSize();

    namespace OnEvent
    {
        void CnageRange();

        // Послать num измерений в Communicator
        void SendDataToCommunicator(Direction::E, int num);
    }
}
