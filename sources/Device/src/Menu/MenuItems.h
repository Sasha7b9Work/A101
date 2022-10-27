// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - � ���� ��������� ������ ��������� ��� ������ ��������� �� ������
    Button(int index, pchar _name_button, pchar _text, bool _highlight, void (*_funcOnPress)(int)) :
        _index(index), name_button(_name_button), text(_text), highlight(_highlight), funcOnPress(_funcOnPress)
    {
    }

    // ��������� ��������� � ������� �����������
    void ToState(int state);

    int GetIndex() const { return _index; }

    void SetText();

    void Highlight();

    static Button empty;

private:

    int _index;
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

private:

    Button *buttons[6];

    static Page *current;
};
