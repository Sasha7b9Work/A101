// 2023/09/22 16:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  Контрол Nextion с типом Text
*/


class CntrlText
{
public:
    CntrlText(pchar _name) : name(_name) { prev_label[0] = '\0'; }
    // Надпись будет выведена только если не совпадает с той, что была нанесена ранее
    void SetLabel(pchar, bool is_different = true);
    void Flash();
    bool IsExist() const { return name[0] != '\0'; }
    pchar LastLabel() const { return prev_label; }
    void SetVisible(bool);
private:
    pchar name;             // Имя, как оно обозначено в программе дисплея
    char prev_label[16];    // Здесь хранится засланное перед этим значение, чтобы не засылать повторно
};
