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

    pchar Name() const { return name; }

private:

    pchar name;
    void (*funcOnPress)(Button *);
};


struct Page
{
    Page(Button **_buttons, void (*_funcDraw)()) : buttons(_buttons), funcDraw(_funcDraw)
    {
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    // ���������� ������ �� 0 �� 5
    Button *GetButton(int index);

    void SetButton(int index, Button *);

    void Draw() { funcDraw(); }

private:

    Button **buttons;

    void (*funcDraw)();

    static Page *current;
};
