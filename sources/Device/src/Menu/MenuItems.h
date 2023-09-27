// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    Button(pchar _name, void (*_funcOnPress)(Button *)) :
        name(_name), funcOnPress(_funcOnPress)
    {
    }

    void Press();

    pchar Name() const { return name; }

private:

    pchar name;
    void (*funcOnPress)(Button *);
};


struct Page
{
    Page(Button **_buttons, void (*_funcOnEnable)(bool), void (*_funcOnDraw)()) :
        buttons(_buttons), funcOnEnable(_funcOnEnable), funcOnDraw(_funcOnDraw)
    {
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    // Возвращает кнопку от 0 до 5
    Button *GetButton(int index);

    void SetButton(int index, Button *);

    void Draw() { funcOnDraw(); }

    Button *GetButton(pchar name);

private:

    Button **buttons;

    // Вызывается при появлении на экране
    void (*funcOnEnable)(bool);

    void (*funcOnDraw)();

    static Page *current;
};
