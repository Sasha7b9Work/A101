// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    Button(pchar _name, pchar _signal, void (*_funcOnPress)(), int _x = -1, int _y = -1) :
        name(_name), signal(_signal), funcOnPress(_funcOnPress), x(_x), y(_y)
    {
    }

    void Press();

    // Сиганл, который присылает кнопка при нажатии
    pchar Signal() const { return signal; }

    void SetText(pchar) const;

    void SetVisible(bool) const;

    // 1 - "нажать", 0 - "отпустить"
    void SetValue(int);

    int GetValue() const { return value; }

    void Draw();

private:

    pchar name;                     // Имя кнопки в редакторе
    pchar signal;                   // Такой сигнал присылает кнопка при нажатии
    void (*funcOnPress)();

    int value = 0;

    const int x;
    const int y;
};


struct Page
{
    Page(Button **_buttons, void (*_funcOnEnter)(), void (*_funcOnDraw)()) :
        buttons(_buttons), funcOnEnter(_funcOnEnter), funcOnDraw(_funcOnDraw)
    {
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    Button *GetButton(int index);

    void SetButton(int index, Button *);

    void Draw();

    Button *GetButton(pchar signal);

private:

    Button **buttons;

    // Вызывается при появлении на экране
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetButtonsCount();
};
