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
        ButtonPress,    // Кнопка без фиксации
        ButtonToggle,   // Кнопка с фиксацией
        Label,          // Текстовая строка
        LabelMeasure,
        Choice,
        Count
    };
};


struct Item
{
    // Если append_to_pool == true - объект нужно добавить в глобальный пул объектов, чтобы страницы управляли их видимостью
    Item(TypeItem::E = TypeItem::Count, const Rect & = Rect(), void (*_funcOnPress)(Item *, bool) = nullptr, bool append_to_pool = true);

    Item &operator=(const Item &);

    virtual void SetParent(Page *page)
    {
        parent = page;
    }

    const Rect &GetRect() const
    {
        return rect;
    }

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

    static void OnEventPress(int, int);
    static void OnEventRelease(int, int);

    ButtonToggle *ToButtonToggle();

    Choice *ToChoice();

    static const int WIDTH_MENU = 253;
    static const int HEIGHT_MENU = 84;

    static int GetCoordX(int col);
    static int GetCoordY(int row);

protected:

    TypeItem::E type;

    Rect rect;
    Page *parent;

    void (*funcOnPress)(Item *, bool);

    bool is_pressed = false;        // Для обычной кнопки переходит в состяние false сразу после отпускания, для кнопки с фиксацией -
                                    // после повторного нажатия

    bool is_shown = false;          // Если true, то надо отрисовывать

    bool need_draw = true;
};


struct ButtonCommon : public Item
{
    ButtonCommon(TypeItem::E, pchar title_ru, pchar title_en, Font::E, const Rect &, void (*_funcOnPress)(Item *, bool), bool append_to_pool = true);

    void SetText(pchar title_ru, pchar title_en);

protected:

    pchar title[2];

    Font::E font;
};


// Кнопка без фиксации (возвращается в отжатое состояние при отпускании)
struct ButtonPress : public ButtonCommon
{
    ButtonPress(pchar title_ru, pchar title_en, Font::E f, const Rect &, void (*_funcOnPress)(Item *, bool), TypeItem::E = TypeItem::ButtonPress, bool append_to_pool = true);

    virtual bool Draw() override;
};


// Кнопка с фиксацией (при нажатии переключается в противоположное состояние, на отпускание реакциии нет)
struct ButtonToggle : public ButtonPress
{
    ButtonToggle(pchar title_ru, pchar title_en, Font::E f, const Rect &_rect, void (*_funcOnPress)(Item *, bool)) :
        ButtonPress(title_ru, title_en, f, _rect, _funcOnPress, TypeItem::ButtonToggle)
    {
    }

    void SetToggled(bool);
};


struct ButtonRange : public ButtonToggle
{
    ButtonRange(pchar title_ru, pchar title_en, int x, int y, void (*_funcOnPress)(Item *, bool)) :
        ButtonToggle(title_ru, title_en, Font::_1, { x, y, 127, 74 }, _funcOnPress)
    {
    }
};


struct ButtonMenuPress : public ButtonPress
{
    ButtonMenuPress(pchar title_ru, pchar title_en, int col_x, int row_y, void (*_funcOnPress)(Item *, bool)) :
        ButtonPress(title_ru, title_en, Font::_1, { Item::GetCoordX(col_x), Item::GetCoordY(row_y), Item::WIDTH_MENU, Item::HEIGHT_MENU }, _funcOnPress)
    {
    }
};


struct ButtonMenuToggle : public ButtonToggle
{
    ButtonMenuToggle(pchar title_ru, pchar title_en, int col_x, int row_y, void (*_funcOnPress)(Item *, bool)) :
        ButtonToggle(title_ru, title_en, Font::_1, { Item::GetCoordX(col_x), Item::GetCoordY(row_y), Item::WIDTH_MENU, Item::HEIGHT_MENU }, _funcOnPress)
    {
    }
};


struct Label : public Item
{
    static const int MAX_LEN = 32;

    // Если append == true - видимостью управляет страница - объект помещается в глобальный пул объектов
    Label(bool append = false, pchar _textRU = "", pchar _textEN = "", const Rect & = Rect(), Font::E = Font::_0, void (*_funcOnPress)(Item *, bool) = EmptyFuncVIemB,
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
    Choice(pchar title_ru, pchar title_en, pchar *_choices,
        int col_x, int row_y, void (*_funcOnPress)(Item *, bool), Font::E = Font::_1);

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

    // Установить текстовое значение, соответствующее текущему выбору
    void SetTextValue();

private:

    int GetCountValue() const;

    pchar titles[Lang::Count];

    pchar *choices;

    ButtonPress button;
    Label       label;

    int index = 0;
};


// Условный размер поля для измерения
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

    void Flash();

    void SetMeasure(const Measure &, int range);

    pchar GetSign() const;

    pchar GetDigits() const;

    pchar GetUnits() const;

    virtual bool Draw() override;

    virtual void SetShown(bool show) override;

private:

    char buf_measure[Label::MAX_LEN];

    Label label_name;
    Label label_sign;
    Label label_digits;
    Label label_units;

    TypeMeasure type_measure;

    void SetMeasure(pchar);

    // after - количество цифр после запятой
    void ConvertRealToText(REAL value, char buffer[Label::MAX_LEN], int after, pchar suffix);

    void DrawSign(pchar);

    void DrawDigits(pchar);

    void DrawUnits(pchar, pchar);

    int GetWidth(SizeMeasure::E, int label);
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

    // Вызывается при появлении на экране
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetItemCount();
};
