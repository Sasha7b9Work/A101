// 2025/11/12 14:41:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"


char *SU::GetWord(pchar data, int num, char out[32], pchar delimit)
{
    Words words(data, delimit);

    if (num <= words.Size())
    {
        std::strcpy(out, words.At(num - 1).c_str());
    }
    else
    {
        out[0] = '\0';
    }

    return out;
}


int SU::GetNumWords(pchar data, pchar delimit)
{
    Words words(data, delimit);

    return words.Size();
}


std::string Words::At(int i) const
{
    if (i < Size())
    {
        return std::string(words[(uint)i].mb_str());
    }

    static const std::string null;
    return null;
}


const wxString &Words::Last() const
{
    if (Size())
    {
        return words[(uint)Size() - 1];
    }

    static const wxString null;
    return null;
}


const wxString &Words::First() const
{
    if (Size())
    {
        return words[0];
    }

    static const wxString null;
    return null;
}
