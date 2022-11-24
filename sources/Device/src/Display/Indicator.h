// 2022/10/28 11:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"


/*
*  ���� ��������� ���������
*/


namespace Indicator
{
    void Init();

    void Update();

    // ����� ���� ������� ������������� ������ ������������� � ����������� �� ������������� �������
    void AutoSize();

    void SetMeasures(float dc, float ac);

    // ���������� ������� ������������
    void SetOverflow();

    // ������� �������� ���
    void SetStatisticsADC(int peak, int ave, int min, int max);

    namespace OnEvent
    {
        void CnageRange();

        // ������� num ��������� � Communicator
        void SendDataToCommunicator(Direction::E, int num);
    }
}
