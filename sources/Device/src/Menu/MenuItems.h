// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    Button(pchar _name, pchar _signal, void (*_funcOnPress)(Button *)) :
        name(_name), signal(_signal), funcOnPress(_funcOnPress)
    {
    }

    void Press();

    // Сиганл, который присылает кнопка при нажатии
    pchar Signal() const { return signal; }

    void SetText(pchar) const;

private:

    pchar name;                     // Имя кнопки в редакторе
    pchar signal;                   // Такой сигнал присылает кнопка при нажатии
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

    Button *GetButton(pchar signal);

private:

    Button **buttons;

    // Вызывается при появлении на экране
    void (*funcOnEnable)(bool);

    void (*funcOnDraw)();

    static Page *current;
};
