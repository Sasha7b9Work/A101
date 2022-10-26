// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Button
{
public:

    Button(int _index, void (*_funcOnPress)(int)) : index(_index), funcOnPress(_funcOnPress) {};

    // Возвращает кнопку для данного присланного из дисплея индекса
    static Button *ForIndex(int);

    // Применить пришедшее с дисплея воздействие
    void ToState(int state) { funcOnPress(state); }

    int GetIndex() const { return index; }

private:

    int index;

    void (*funcOnPress)(int);
};
