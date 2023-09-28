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

    // ������� �������� ���
    void SetStatisticsADC(int peak, int ave, int min, int max);

    void ConvertDoubleToText(float value, char buffer[TextString::MAX_LEN], int after, pchar suffix);

    namespace OnEvent
    {
        void CnageRange();

        // ������� num ��������� � Communicator
        void SendDataToCommunicator(Direction::E, int num);
    }
}
