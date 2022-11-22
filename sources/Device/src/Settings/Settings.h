// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/CalibrationSettings.h"


struct Settings
{
    uint size;                  // ����� ������ �������� - ��� �������� ����, ��� ������ �������������
    uint crc32;                 // ����� ����������� ����� - ��� �������� ������������ ����������
    bool middle_of_3;           // �������� ���������� �������
    bool smooth;                // ��������� ����������� �� �������� ������
    bool enabled_zero;          // "������ ����"
    bool firLPF;                // ������� �������� ���

    void Save();
    void Load();

    uint CalculateCRC32();

    void Store();
    void Restore();
};


extern Settings set;
