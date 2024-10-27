// 2022/10/19 09:03:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace DiagramInput
{
    void InstallData();

    // ��� ��������������� �������. ����� ������������ ��� �������
    void InstallData2(float data[1024]);

    void Draw();

    void Reset(bool clear);

    // ���� true, �� �� ���� ��������� ���������, ��� ���������� �������� �������� ������� - ����� 300 ��
    bool DataInstalled();
}
