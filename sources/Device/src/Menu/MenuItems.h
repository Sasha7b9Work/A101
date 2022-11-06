// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct Button
{
public:

    // _highlight - � ���� ��������� ������ ��������� ��� ������ ��������� �� ������
    Button(pchar _name_button, pchar _text, bool _highlight, void (*_funcOnPress)(Button *)) :
        name(_name_button), text(_text), highlight(_highlight), funcOnPress(_funcOnPress)
    {
    }

    void Press();

    void SetText();

    void Highlight();

    void SetHighlight(bool);

    void ToggleHighlight();

    // ���������� true, ���� ������ ��������� � ���������� ���������
    bool IsHightlight() const { return highlight; }

    void Enable();

    void Disable();

    // ��������� ������ ������ ��������� ������
    void SetText(pchar text);

    pchar GetText() const { return text; }

    bool IsExist() const { return text[0] != '\0'; }

private:

    pchar name;
    pchar text;                         // ���� ����� ����, �� ������ �� ����������
    bool highlight;
    void (*funcOnPress)(Button *);
};


struct Page
{
    Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5);

    static Page *Current() { return current; }

    void SetAsCurrent();

    // ���������� ������ �� 0 �� 5
    Button *GetButton(int index);

private:

    Button *buttons[7];

    static Page *current;

    static Button btnMenu;
};
