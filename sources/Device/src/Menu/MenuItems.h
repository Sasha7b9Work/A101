// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    Button(int _index, pchar _name_button, pchar _text, void (*_funcOnPress)(int)) :
        index(_index), name_button(_name_button), text(_text), funcOnPress(_funcOnPress)
    {
    }

    // Возвращает кнопку для данного присланного из дисплея индекса
    static Button *ForIndex(int);

    // Применить пришедшее с дисплея воздействие
    void ToState(int state)
    {
        Log::Write("Button %d to state %d", index, state);
        funcOnPress(state);
    }

    int GetIndex() const { return index; }

    void SetText();

private:

    int index;
    pchar name_button;
    pchar text;
    void (*funcOnPress)(int);
};
