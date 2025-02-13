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
struct Choice;


struct TypeItem
{
    enum E
    {
        Another,
        ButtonPress,    // ������ ��� ��������
        ButtonToggle,   // ������ � ���������
        Label,          // ��������� ������
        LabelMeasure,
        Choice,
        Count
    };
};


struct Item
{
    // ���� append_to_pool == true - ������ ����� �������� � ���������� ��� ��������, ����� �������� ��������� �� ����������
    Item(TypeItem::E = TypeItem::Count, const Rect & = Rect(), void (*_funcOnPress)(Item *, bool) = nullptr, bool append_to_pool = true);

    Item &operator=(const Item &);

    virtual void SetParent(Page *page)
    {
        parent = page;
    }

    // ���������� ������ ���������
    const Rect &GetRect() const
    {
        return rect;
    }

    // ���������� ������ �������
    virtual Rect GetRectForPress() const;

    virtual void SetShown(bool show);
    bool IsShown() const;

    virtual bool Draw() = 0;
    virtual void Refresh()
    {
        need_draw = true;
        Draw();
    }

    virtual void Press();
    virtual void Release();

    bool IsPressed() const;

    void SetEnabled(bool enable);
    bool IsEnabled() const { return enabled; }

    static void OnEventPress(int, int);
    static void OnEventRelease(int, int);

    ButtonToggle *ToButtonToggle();
    ButtonPress *ToButtonPress();

    Choice *ToChoice();

    static const int WIDTH_MENU = 253;
    static const int HEIGHT_MENU = 84;

    static int GetCoordX(int col);
    static int GetCoordY(int row);

    void SetCoord(const Coord &);
    Coord GetCoord() const;

protected:

    TypeItem::E type;

    Rect rect;
    Page *parent;

    void (*funcOnPress)(Item *, bool);

    bool is_pressed = false;        // ��� ������� ������ ��������� � �������� false ����� ����� ����������, ��� ������ � ��������� -
                                    // ����� ���������� �������

    bool is_shown = false;          // ���� true, �� ���� ������������

    bool need_draw = true;

    bool enabled = true;
};


struct ButtonCommon : public Item
{
    ButtonCommon(TypeItem::E, pchar title_ru, pchar title_en, Font::E, const Rect &, void (*_funcOnPress)(Item *, bool), bool append_to_pool = true);

    void SetText(pchar title_ru, pchar title_en);

protected:

    pchar title[2];

    Font::E font;
};


// ������ ��� �������� (������������ � ������� ��������� ��� ����������)
struct ButtonPress : public ButtonCommon
{
    // tickness - ������� �������
    ButtonPress(pchar title_ru, pchar title_en, Font::E f, const Rect &, void (*_funcOnPress)(Item *, bool),
        int _tickness = 3, TypeItem::E = TypeItem::ButtonPress, bool append_to_pool = true);

    virtual bool Draw() override;

    void SetExtendedHeightPress()
    {
        is_extened_height_press = true;
    }

    virtual Rect GetRectForPress() const override;

    // ���������� ������ ������ � ������ ����, ����������� ��� ��� ��� (��������� ��� � ������, ���� � ��������� � ������)
    // ���� for_erase == true, �� ������������� ����� ��� �������� ������ � ������ �������
    Rect GetDrawRect(bool for_erase) const;

    bool IsBigRectangleForDraw() const;

    // ����� ������� ���������� "����������" ������
    void SetInactiveColor();

    // ����� ������� ���������� "��������" ������
    void SetActiveColor();

protected:

    int tickness = 0;
    bool is_extened_height_press = false;   // ���� true, �� ���� ������������ ��������� � ������ � ��� ���� ����
    bool prev_big_rectangle = true;
    bool inactive_color = false;
};


// ������ � ��������� (��� ������� ������������� � ��������������� ���������, �� ���������� �������� ���)
struct ButtonToggle : public ButtonPress
{
    ButtonToggle(pchar title_ru, pchar title_en, Font::E f, const Rect &_rect, void (*_funcOnPress)(Item *, bool), int _tickness = 3) :
        ButtonPress(title_ru, title_en, f, _rect, _funcOnPress, _tickness, TypeItem::ButtonToggle)
    {
    }

    void SetToggled(bool toggle, bool execute = true);
};


struct ButtonRange : public ButtonToggle
{
    ButtonRange(pchar title_ru, pchar title_en, int x, int y, void (*_funcOnPress)(Item *, bool)) :
        ButtonToggle(title_ru, title_en, Font::_1_GB42b, { x, y, 127, 74 }, _funcOnPress)
    {
    }
};


struct ButtonMenuPress : public ButtonPress
{
    ButtonMenuPress(pchar title_ru, pchar title_en, int col_x, int row_y, void (*_funcOnPress)(Item *, bool) = nullptr) :
        ButtonPress(title_ru, title_en, Font::_1_GB42b, { Item::GetCoordX(col_x), Item::GetCoordY(row_y), Item::WIDTH_MENU, Item::HEIGHT_MENU }, _funcOnPress)
    {
    }
};


struct ButtonMenuToggle : public ButtonToggle
{
    ButtonMenuToggle(pchar title_ru, pchar title_en, int col_x, int row_y, void (*_funcOnPress)(Item *, bool)) :
        ButtonToggle(title_ru, title_en, Font::_1_GB42b, { Item::GetCoordX(col_x), Item::GetCoordY(row_y), Item::WIDTH_MENU, Item::HEIGHT_MENU }, _funcOnPress)
    {
    }
};


struct Label : public Item
{
    static const int MAX_LEN = 32;

    // ���� append == true - ���������� ��������� �������� - ������ ���������� � ���������� ��� ��������
    Label(bool append = false, pchar _textRU = "", pchar _textEN = "", const Rect & = Rect(), Font::E = Font::_0_GB34b, void (*_funcOnPress)(Item *, bool) = EmptyFuncVIemB,
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
    virtual bool Draw() override;

protected:

    char  text[Lang::Count][MAX_LEN];
    int   font;
    bool  h_aligned;
    bool  v_aligned;
    Color colorText;
    Color colorBack;
};


struct Choice : public Item
{
    Choice(pchar title_ru, pchar title_en, uint8 *choice, pchar *_names, int col_x, int row_y, void (_funcOnPress)(Item *, bool) = nullptr, Font::E = Font::_1_GB42b);

    virtual void SetParent(Page *) override;

    virtual bool Draw() override;

    virtual void SetShown(bool) override;

    virtual void Press() override;

    virtual void Release() override
    {
        need_draw = true;

        button.Release();

        button.Refresh();
    }

    // ���������� ��������� ��������, ��������������� �������� ������
    void SetTextValue();

    void SetTitles(pchar title_ru, pchar title_en);

    ButtonPress &GetButton()
    {
        return button;
    }

private:

    pchar      *names;
    ButtonPress button;
    Label       label;
    uint8      *choice;
    int        count_values;
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
    LabelMeasure(TypeMeasure::E, SizeMeasure::E, int _x, int _y, void (*_funcOnPress)(Item *, bool) = EmptyFuncVIemB);

    void Reset();

    virtual void Refresh() override;

    // ����� ������� �� ���� ���������. � units ����� �������
    void SetMeasure(pchar message_ru, pchar message_en);

    void SetMeasure(const Measure &, int range);

    pchar GetSign() const;

    pchar GetDigits() const;

    pchar GetUnits() const;

    void SetCoord(const Coord &);

    virtual bool Draw() override;

    virtual void SetShown(bool show) override;

private:

    char buf_measure[Label::MAX_LEN];

    Label label_name;
    Label label_sign;
    Label label_digits;
    Label label_units;

    TypeMeasure type_measure;

    void SetMeasure(pchar, pchar units_ru, pchar units_en);

    // after - ���������� ���� ����� �������
    void ConvertRealToText(REAL value, char buffer[Label::MAX_LEN], int after, int num_digits);

    void DrawSign(pchar);

    void DrawDigits(pchar);

    int GetWidth(SizeMeasure::E, int label);

    REAL frequency = 0.0;
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

    void Refresh();

private:

    Item **items;

    // ���������� ��� ��������� �� ������
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetItemCount();
};
