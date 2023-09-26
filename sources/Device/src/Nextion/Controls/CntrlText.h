// 2023/09/22 16:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  Контрол Nextion с типом Text
*/


class CntrlText
{
public:
    CntrlText(pchar _name) : name(_name) { prev_label[0] = '\0'; }
    void SetLabel(pchar);
    bool IsExist() const { return name[0] != '\0'; }
private:
    pchar name;
    char prev_label[16];
};
