// 2025/11/12 14:37:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Words
{
    Words(const wxString &line, pchar delimiters = " ")
    {
        words = wxStringTokenize(line, delimiters);
    }

    int Size() const
    {
        return (int)words.size();
    }
    void Clear()
    {
        words.clear();
    }
    void Push(const std::string &word)
    {
        words.push_back(word);
    }
    std::string At(int i) const;
    wxString operator[](int i) const
    {
        return At(i);
    }
    const wxString &Last() const;
    const wxString &First() const;

    // true, если содержит слово
    bool Consist(const wxString &) const;

private:

    wxArrayString words;
};


namespace SU
{
    // Нумерация начинается с 1
    char *GetWord(pchar data, int num, char out[32], pchar delimit = " \n\0");

    int GetNumWords(pchar data, pchar delimit = " \n");
}
