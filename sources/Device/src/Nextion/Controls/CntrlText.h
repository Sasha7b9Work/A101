// 2023/09/22 16:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   онтрол Nextion с типом Text
*/


class CntrlText
{
public:
    CntrlText(pchar _name) : name(_name) { prev_label[0] = '\0'; }
    // Ќадпись будет выведена только если не совпадает с той, что была нанесена ранее
    void SetLabel(pchar, bool is_different = true);
    void Flash();
    bool IsExist() const { return name[0] != '\0'; }
private:
    pchar name;

    char prev_label[16];    // «десь хранитс€ засланное перед этим значение, чтобы не засылать повторно
};
