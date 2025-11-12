// 2025/11/12 10:38:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace File
{
    bool Create(const wxString &);

    uint GetVersion();

    wxString GetDateBuild();

    // Является файлом прошивки
    bool IsValid();
}
