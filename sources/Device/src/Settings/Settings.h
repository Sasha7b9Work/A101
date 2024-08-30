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


// Что выводится на дисплей
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
    uint size;                  // Здесь размер настроек - для проверки того, что версии соответствуют
    uint crc32;                 // Здесь контрольная сумма - для проверки правильности сохранения

    bool middle_of_3;           // Влючение медианного фильтра
    bool smooth;                // Включение сглаживания по соседним точкам
    bool firLPF;                // Включён цифровой ФНЧ

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
