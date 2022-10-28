// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    Button(pchar _name_button, pchar _text, bool _highlight, void (*_funcOnPress)(int)) :
        name_button(_name_button), text(_text), highlight(_highlight), funcOnPress(_funcOnPress)
    {
    }

    // Применить пришедшее с дисплея воздействие
    void ToState(int state);

    void SetText();

    void Highlight();

    static Button empty;

private:

    pchar name_button;
    pchar text;
    bool highlight;
    void (*funcOnPress)(int);
};


struct Page
{
    Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5);

    static Page *Current() { return current; }

    void SetAsCurrent();

    // Возвращает кнопку от 0 до 5
    Button *GetButton(int index);

    void Init();

private:

    Button *buttons[6];

    static Page *current;
};
