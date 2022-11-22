// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/CalibrationSettings.h"


struct Settings
{
    uint size;                  // Здесь размер настроек - для проверки того, что версии соответствуют
    uint crc32;                 // Здесь контрольная сумма - для проверки правильности сохранения
    bool middle_of_3;           // Влючение медианного фильтра
    bool smooth;                // Включение сглаживания по соседним точкам
    bool enabled_zero;          // "Влючён ноль"
    bool firLPF;                // Включён цифровой ФНЧ

    void Save();
    void Load();

    uint CalculateCRC32();

    void Store();
    void Restore();
};


extern Settings set;
