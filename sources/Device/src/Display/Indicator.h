// 2022/10/28 11:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"
#include "Display/Controls/TextString.h"


/*
*  ���� ��������� ���������
*/


namespace Indicator
{
    void Update();

    // ����� ���� ������� ������������� ������ ������������� � ����������� �� ������������� �������
    void AutoSize();

    namespace OnEvent
    {
        void CnageRange();

        // ������� num ��������� � Communicator
        void SendDataToCommunicator(Direction::E, int num);
    }
}
