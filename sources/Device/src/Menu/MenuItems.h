// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Label
{
    struct Size
    {
        enum E
        {
            Big,
            Small,
            Count
        };
    };

    // Нараметры - имена соответсвующих контролов в прошивке дисплея
    Label(pchar name_big, pchar name_small, pchar text);

    void SetSize(Size::E);

    void SetText(pchar);

private:

    Size::E size;

    pchar text;

    pchar name_big;
    pchar name_small;

    // Включить/выключить контрол с именем name
    void Enable(pchar name, bool);
};


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


/*
*   У каждой страницы есть шесть кнопок, которые являются разными для каждой страницы.
*   Кнопка "Меню" и остальные элементы являются общими
*/


struct Page
{
    Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5);

    static Page *Current() { return current; }

    void SetAsCurrent();

    // Возвращает кнопку от 0 до 5
    Button *GetButton(int index);

    void Init();

    static Label labelDC;           // Надпись "DC"
    static Label textDC;            // Числовое значение DC

    static Label labelAC;           // Надпись "AC"
    static Label textAC;            // Числовое значение AC

private:

    Button *buttons[6];

    static Page *current;
};
