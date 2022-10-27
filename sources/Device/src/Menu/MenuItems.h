// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    Button(int _index, pchar _name_button, pchar _text, void (*_funcOnPress)(int)) :
        index(_index), name_button(_name_button), text(_text), highlight(false), funcOnPress(_funcOnPress)
    {
    }

    // Возвращает кнопку для данного присланного из дисплея индекса
    static Button *ForIndex(int);

    // Применить пришедшее с дисплея воздействие
    void ToState(int state);

    int GetIndex() const { return index; }

    void SetText();

    void Highlight(bool);

    bool IsHightlighted() const { return highlight; }

    static Button empty;

private:

    int index;
    pchar name_button;
    pchar text;
    bool highlight;
    void (*funcOnPress)(int);
};


/*
*   У каждой страницы есть шесть кнопок, которые являются разными для каждой страницы.
*   Кнопка "Меню" и остальные элементы являются общими
*/


struct Page
{
    Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5);

    static Page *Current();

    void SetAsCurrent();

    // Возвращает кнопку от 0 до 5
    Button *GetButton(int index);

private:

    Button *buttons[6];
};
