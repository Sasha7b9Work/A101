// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/CalibrationSettings.h"


struct Lang
{
    enum E
    {
        RU,
        EN,
        Count
    };
};


// ��� ��������� �� �������
struct MeasuresOnDisplay
{
    enum E
    {
        AC_DC,
        AC,
        DC,
        Count
    };

    static void Set(E v) { current = v; }

    static bool IsAC_DC() { return current == AC_DC; }
    static bool IsAC() { return current == AC; }
    static bool IsDC() { return current == DC; }

    static E current;
};


struct Settings
{
    uint size;                  // ����� ������ �������� - ��� �������� ����, ��� ������ �������������
    uint crc32;                 // ����� ����������� ����� - ��� �������� ������������ ����������

    bool middle_of_3;           // �������� ���������� �������
    bool smooth;                // ��������� ����������� �� �������� ������
    bool firLPF;                // ������� �������� ���

    bool en_Imax;
    bool en_Imin;
    bool en_Iampl;
    bool en_Ipp;

    uint serial_number;

    Lang::E lang;

    void Save();
    void Load();

    uint CalculateCRC32() const;
    bool IsEqual(const Settings *) const;

    const uint8 *PointerToFirstData() const;
    uint SizeData() const;

    struct Storage
    {
        static void Store(const Settings &);
        static void Restore(Settings &);
    };
};


extern Settings set;
