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

    void Store() const;
    void Restore();

    uint CalculateCRC32() const;
    bool IsEqual(const Settings *) const;

    const uint8 *PointerToFirstData() const;
    uint SizeData() const;
};


extern Settings set;
