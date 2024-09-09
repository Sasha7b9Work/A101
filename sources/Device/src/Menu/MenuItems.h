// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"
#include "Nextion/Controls.h"
#include "Utils/Math.h"
#include "Nextion/Colors.h"
#include "Settings/Settings.h"
#include "Ampermeter/Ampermeter.h"


struct Page;
struct ButtonRange;
struct ButtonPress;
struct ButtonToggle;


struct TypeItem
{
    enum E
    {
        Another,
        ButtonPress,    // ������ ��� ��������
        ButtonToggle,   // ������ � ���������
        Label,          // ��������� ������
        LabelMeasure,
        Count
    };
};


struct Item
{
    // ���� append_to_pool == true - ������ ����� �������� � ���������� ��� ��������, ����� �������� ��������� �� ����������
    Item(TypeItem::E = TypeItem::Count, const Rect & = Rect(), void (*_funcOnPress)(Item *) = nullptr, bool append_to_pool = true);

    Item &operator=(const Item &);

    void SetParent(Page *page)
    {
        parent = page;
    }

    const Rect &GetRect() const
    {
        return rect;
    }

    virtual void SetShown(bool show);
    bool IsShown() const;

    virtual void Draw() = 0;

    virtual void Press();
    virtual void Release();

    static void OnEventPress(int, int);
    static void OnEventRelease(int, int);

    ButtonRange *ToButtonRange();
    ButtonPress *ToButtonPress();
    ButtonToggle *ToButtonToggle();

protected:

    TypeItem::E type;

    Rect rect;
    Page *parent;

    void (*funcOnPress)(Item *);

    bool is_pressed = false;        // ��� ������� ������ ��������� � �������� false ����� ����� ����������, ��� ������ � ��������� -
                                    // ����� ���������� �������

    bool is_shown = false;          // ���� true, �� ���� ������������
};


struct ButtonCommon : public Item
{
    ButtonCommon(TypeItem::E, pchar title_ru, pchar title_en, Font::E, const Rect &, void (*_funcOnPress)(Item *));

    // 1 - "������", 0 - "���������"
    virtual void SetValue(bool);

    bool IsPressed() const;

    void SetText(pchar title_ru, pchar title_en);

    static const int WIDTH_MENU = 210;
    static const int HEIGHT_MENU = 84;

    static int GetCoordX(int col);
    static int GetCoordY(int row);

protected:

    pchar title[2];

    Font::E font;
};


// ������ ��� �������� (������������ � ������� ��������� ��� ����������)
struct ButtonPress : public ButtonCommon
{
    ButtonPress(pchar title_ru, pchar title_en, Font::E f, const Rect &, void (*_funcOnPress)(Item *), TypeItem::E = TypeItem::ButtonPress);

    virtual void Draw() override;
};


// ������ � ��������� (��� ������� ������������� � ��������������� ���������, �� ���������� �������� ���)
struct ButtonToggle : public ButtonPress
{
    ButtonToggle(pchar title_ru, pchar title_en, Font::E f, const Rect &_rect, void (*_funcOnPress)(Item *)) :
        ButtonPress(title_ru, title_en, f, _rect, _funcOnPress, TypeItem::ButtonToggle)
    {
    }
};


struct ButtonRange : public ButtonToggle
{
    ButtonRange(pchar title_ru, pchar title_en, int x, int y, void (*_funcOnPress)(Item *)) :
        ButtonToggle(title_ru, title_en, Font::_1, { x, y, 127, 74 }, _funcOnPress)
    {
    }
};


struct Label : public Item
{
    static const int MAX_LEN = 32;

    // ���� append == true - ���������� ��������� �������� - ������ ���������� � ���������� ��� ��������
    Label(bool append = false, pchar _textRU = "", pchar _textEN = "", const Rect & = Rect(), Font::E = Font::_0, void (*_funcOnPress)(Item *) = EmptyFuncVIem,
        const Color &_colorText = Color::White, const Color &_colorBack = Color::Count, bool _h_aligned = false, bool _v_align = true);

    Label &operator=(const Label &);

    void SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN]);

    void SetX(int _x)
    {
        rect.x = (int16)_x;
    }
    int GetX() const
    {
        return rect.x;
    }
    int GetY() const
    {
        return rect.y;
    }
    int GetWidth() const
    {
        return rect.width;
    }
    pchar Text() const;
    pchar Text(Lang::E lang) const
    {
        return text[lang];
    }
    virtual void Draw() override;

protected:

    char  text[Lang::Count][MAX_LEN];
    int   font;
    bool  h_aligned;
    bool  v_aligned;
    Color colorText;
    Color colorBack;
};


// �������� ������ ���� ��� ���������
struct SizeMeasure
{
    enum E
    {
        Big,
        Small,
        Calibration,
        Count
    };
};


struct LabelMeasure : public Label
{
    LabelMeasure(TypeMeasure::E, SizeMeasure::E, int _x, int _y, void (*_funcOnPress)(Item *) = EmptyFuncVIem);

    void Reset();

    void Flash();

    void SetMeasure(const Measure &, int range);

    pchar GetSign() const;

    pchar GetDigits() const;

    pchar GetUnits() const;

    virtual void Draw() override;

    virtual void SetShown(bool show) override;

private:

    char buf_measure[Label::MAX_LEN];

    Label label_name;
    Label label_sign;
    Label label_digits;
    Label label_units;

    TypeMeasure type_measure;

    void SetMeasure(pchar);

    // after - ���������� ���� ����� �������
    void ConvertRealToText(REAL value, char buffer[Label::MAX_LEN], int after, pchar suffix);

    void DrawSign(pchar);

    void DrawDigits(pchar);

    void DrawUnits(pchar, pchar);
};


struct Page
{
    Page(Item **_items, void (*_funcOnEnter)(), void (*_funcOnDraw)()) :
        items(_items), funcOnEnter(_funcOnEnter), funcOnDraw(_funcOnDraw)
    {
        for (int i = 0; i < GetItemCount(); i++)
        {
            GetItem(i)->SetParent(this);
        }
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    Item *GetItem(int index);

    void SetItem(int index, Item *);

    void Draw();

private:

    Item **items;

    // ���������� ��� ��������� �� ������
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetItemCount();
};
