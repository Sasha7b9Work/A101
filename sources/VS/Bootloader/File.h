// 2025/11/12 10:38:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace File
{
    bool Create(const wxString &);

    void Reset();

    uint GetVersion();

    wxString GetDateBuild();

    // Является файлом прошивки
    bool IsValid();

    uint GetSize();

    uint GetCRC32();

    // Возвращает данные в текущей позиции указателя и передвигает указатель на num_bytes
    uint8 *CurrentData(uint num_bytes);

    // Когда все байты получены CurrentData, эта функция вернёт true
    bool AllBytesCompleted();
}
