// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - � ���� ��������� ������ ��������� ��� ������ ��������� �� ������
    Button(pchar _name, void (*_funcOnPress)(Button *)) :
        name(_name), funcOnPress(_funcOnPress)
    {
    }

    void Press();

private:

    pchar name;
    void (*funcOnPress)(Button *);
};


struct Page
{
    Page(Button **_buttons)
    {
        buttons = _buttons;
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    // ���������� ������ �� 0 �� 5
    Button *GetButton(int index);

    void SetButton(int index, Button *);

private:

    Button **buttons;

    static Page *current;
};
