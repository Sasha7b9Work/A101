// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    float Gain(int range);      // ����������� ��������
    int Zero(int range);
};


struct Settings
{
    bool middle_of_3;           // �������� ���������� �������
    bool smooth;                // ��������� ����������� �� �������� ������
    bool enabled_zero;          // "������ ����"
    bool firLPF;                // ������� �������� ���

    CalibrationSettings cal;
};


extern Settings set;
