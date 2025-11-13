// 2025/11/13 08:46:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// ѕросто место дл€ хранени€ строки
struct ParseBuffer
{
    char data[256];

    int GetLength() const;

    // true, строка соотвествует
    bool IsEqual(pchar) const;

    // ¬озвращает true, если здесь хранитс€ параметр (две строки, разделЄнные символом '=')
    bool IsParameter(pchar param) const;
};


namespace SU
{
    // Ќумераци€ начинаетс€ с 1
    char *GetWord(pchar data, int num, ParseBuffer *);

    // Ќумераци€ начинаетс€ с 1
    char *GetString(pchar data, int num, char *out, pchar *first_symbol = nullptr);

    // —читывает значение int из дес€тичного ASCII-представлени€
    bool AtoIntDec(pchar, int *value);

    // —читывает значение uint из дес€тичного или шестнадцатиричного ASCII-представлени€
    bool AtoUInt(pchar, uint *value, int radix);

    uint CalculateCRC32(const void *buffer, int size);
}
