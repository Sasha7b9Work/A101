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

    // ��������� - ����� �������������� ��������� � �������� �������
    Label(pchar name_big, pchar name_small, pchar text);

    void SetSize(Size::E);

    void SetText(pchar);

private:

    Size::E size;

    pchar text;

    pchar name_big;
    pchar name_small;

    // ��������/��������� ������� � ������ name
    void Enable(pchar name, bool);
};


struct Button
{
public:

    // _highlight - � ���� ��������� ������ ��������� ��� ������ ��������� �� ������
    Button(pchar _name_button, pchar _text, bool _highlight, void (*_funcOnPress)(int)) :
        name_button(_name_button), text(_text), highlight(_highlight), funcOnPress(_funcOnPress)
    {
    }

    // ��������� ��������� � ������� �����������
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
*   � ������ �������� ���� ����� ������, ������� �������� ������� ��� ������ ��������.
*   ������ "����" � ��������� �������� �������� ������
*/


struct Page
{
    Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5);

    static Page *Current() { return current; }

    void SetAsCurrent();

    // ���������� ������ �� 0 �� 5
    Button *GetButton(int index);

    void Init();

    static Label labelDC;           // ������� "DC"
    static Label textDC;            // �������� �������� DC

    static Label labelAC;           // ������� "AC"
    static Label textAC;            // �������� �������� AC

private:

    Button *buttons[6];

    static Page *current;
};
