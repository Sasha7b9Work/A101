// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include "Hardware/Timer.h"
#include "Utils/String.h"
#include "Nextion/Display.h"
#include "Ampermeter/InputRelays.h"
#include <cstring>
#include <cstdio>


using namespace std;


Page *Page::current = PageMain::self;


namespace PoolItems
{
    static const int MAX_ITEMS = 1024;                  // Здесь хранятся все созданные кнопки (со всех страниц)
    static Item *items[MAX_ITEMS];
    static int num_items = 0;                         // Столько создано кнопок в данный момент

    static void SetAllInactive()
    {
        for (int i = 0; i < num_items; i++)
        {
            items[i]->SetShown(false);
        }
    }

    static void AppendNewItem(Item *item)
    {
        items[num_items] = item;

        if (num_items++ >= MAX_ITEMS)
        {
            LOG_ERROR_TRACE("The items buffer is full");
        }
    }

    static bool Consist(const Item *item)
    {
        for (int i = 0; i < num_items; i++)
        {
            if (item == items[i])
            {
                return true;
            }
        }

        return false;
    }

    Item *GetItemDraw(int x, int y);

    // Возвращает Item по фигуре отрисовки
    Item *GetItemDraw(int x, int y)
    {
        for (int i = 0; i < num_items; i++)
        {
            Item *item = items[i];

            if (item->IsShown() && item->GetRect().Intersect(x, y))
            {
                return item;
            }
        }

        return nullptr;
    }

    // Возвращает Item по фигуре нажатия
    static Item *GetItemPress(int x, int y)
    {
        for (int i = 0; i < num_items; i++)
        {
            Item *item = items[i];

            if (item->IsShown() && item->GetRectForPress().Intersect(x, y))
            {
                return item;
            }
        }

        return nullptr;
    }
}


Rect Item::GetRectForPress() const
{
    return rect;
}


Rect ButtonPress::GetRectForPress() const
{
    if (is_extened_height_press)
    {
        Rect result = rect;

        result.height *= 2;

        return result;
    }
    else
    {
        return rect;
    }
}


bool Item::IsShown() const
{
    if (PoolItems::Consist(this))
    {
        return (Page::Current() == parent) && is_shown;
    }
    else
    {
        return is_shown;
    }
}


void Item::Press()
{
    if (type == TypeItem::ButtonToggle)
    {
        is_pressed = !is_pressed;
    }
    else
    {
        is_pressed = true;
    }

    need_draw = true;

    if (funcOnPress)
    {
        funcOnPress(this, is_pressed);
    }
}


void Item::Release()
{
    if (type != TypeItem::ButtonToggle)
    {
        is_pressed = false;

        need_draw = true;

        if (funcOnPress)
        {
            funcOnPress(this, false);
        }
    }
}


void Item::SetEnabled(bool set_enabled)
{
    if (set_enabled)
    {
        if (IsEnabled())
        {
            return;
        }

        enabled = true;

        SetShown(true);

        if (Page::Current() == parent)
        {
            Draw();
        }
    }
    else
    {
        if (!IsEnabled())
        {
            return;
        }

        SetShown(false);

        if (Page::Current() == parent)
        {
            Draw();
        }

        enabled = false;
    }
}


void ButtonToggle::SetToggled(bool toggled, bool execute)
{
    is_pressed = toggled;

    need_draw = true;

    if (execute && funcOnPress)
    {
        funcOnPress(this, is_pressed);
    }
}


Item::Item(TypeItem::E _type, const Rect &_rect, void (*_funcOnPress)(Item *, bool), bool append) :
    type(_type), rect(_rect), funcOnPress(_funcOnPress)
{
    if (append)
    {
        PoolItems::AppendNewItem(this);
    }
}


Item &Item::operator=(const Item &rhs)
{
    type = rhs.type;
    rect = rhs.rect;
    parent = rhs.parent;
    funcOnPress = rhs.funcOnPress;
    is_pressed = rhs.is_pressed;
    is_shown = rhs.is_shown;

    return *this;
}


ButtonCommon::ButtonCommon(TypeItem::E _type, pchar title_ru, pchar title_en, Font::E _f, const Rect &_rect, void (*_funcOnPress)(Item *, bool), bool _append_to_pool) :
    Item(_type, _rect, _funcOnPress, _append_to_pool),
    font(_f)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;
}


void ButtonCommon::SetText(pchar title_ru, pchar title_en)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;
}


Choice::Choice(pchar title_ru, pchar title_en, uint8 *_choice, pchar *_names, int x, int y, void (*_funcOnPress)(Item *, bool), Font::E) :
    Item(TypeItem::Choice, { Item::GetCoordX(x), Item::GetCoordY(y), Item::WIDTH_MENU, Item::HEIGHT_MENU }, _funcOnPress),
    names(_names),
    button(title_ru, title_ru, Font::_1_GB42b, { Item::GetCoordX(x), Item::GetCoordY(y), Item::WIDTH_MENU, Item::HEIGHT_MENU}, _funcOnPress, 3, TypeItem::ButtonPress, false),
    label(false, "", "", { Item::GetCoordX(x) + 10 + Item::WIDTH_MENU, Item::GetCoordY(y) , Item::WIDTH_MENU, Item::HEIGHT_MENU} ),
    choice(_choice)
{
    titles[Lang::RU] = title_ru;
    titles[Lang::EN] = title_en;

    SetTextValue();
}


void Choice::SetTextValue()
{
    pchar *value_ru = names + (*choice) * 2;
    pchar *value_en = value_ru + 1;

    label.SetText(*value_ru, *value_en);

    if (label.IsShown())
    {
        need_draw = true;
    }
}


void Choice::Press()
{
    button.Press();

    button.Refresh();

    (*choice)++;

    if ((*choice) == GetCountValue())
    {
        *choice = 0;
    }

    SetTextValue();
}


int Choice::GetCountValue() const
{
    pchar *pointer = names;

    int counter = 0;

    while ((*pointer++)[0] != '\0')
    {
        counter++;
    }

    return counter / 2;
}


void Item::OnEventPress(int x, int y)
{
    Item *item = PoolItems::GetItemPress(x, y);

    if (item)
    {
        item->Press();
    }
}


void Item::OnEventRelease(int x, int y)
{
    Item *item = PoolItems::GetItemPress(x, y);

    if (item)
    {
        item->Release();
    }
}


void Menu::Init()
{
    Display::Clear();

    PageMain::SetMeasuresOnDisplay(set.meas_on_display.Current());
    PageMain::SetRange(2);
    PageMain::SetRange(3);

    PageMain::self->SetAsCurrent();
}


Item *Page::GetItem(int index)
{
    return items[index];
}


void Page::SetItem(int index, Item *item)
{
    items[index] = item;
}


void Page::SetAsCurrent()
{
    PoolItems::SetAllInactive();

    current = this;

    Display::Clear();

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->SetShown(true);
    }

    if (current->funcOnEnter)
    {
        current->funcOnEnter();
    }
}


void Page::Draw()
{
    if (funcOnDraw)
    {
        funcOnDraw();
    }

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->Draw();
    }
}


void Page::Refresh()
{
    if (funcOnDraw)
    {
        funcOnDraw();
    }

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->Refresh();
    }
}


int Page::GetItemCount()
{
    int count = 0;

    Item **item = &items[0];

    while (*item++)
    {
        count++;
    }

    return count;
}


ButtonPress::ButtonPress(pchar title_ru, pchar title_en, Font::E _f, const Rect &_rect, void (*_funcOnPress)(Item *, bool), int _tickness, TypeItem::E _type, bool _append_to_pool) :
    ButtonCommon(_type, title_ru, title_en, _f, _rect, _funcOnPress, _append_to_pool),
    tickness(_tickness)
{
}


bool Item::IsPressed() const
{
    return is_pressed;
}


bool ButtonPress::Draw()
{
    if (need_draw)
    {
        int16 x = rect.x;
        int16 y = rect.y;
        int16 width = rect.width;
        int16 height = rect.height;

        if (IsShown())
        {
            for (int i = 0; i < tickness; i++)
            {
                Nextion::DrawRect({ x + i, y + i, width - 1 - i * 2, height - 1 - i * 2}, Color::White);
            }

            Nextion::DrawString({ x + tickness, y + tickness, width - tickness * 2, height - tickness * 2 }, font,
                Color::White,
                is_pressed ? Color::ButtonPress : Color::Background,
                title[set.lang], true, true);
        }
        else
        {
            Nextion::FillRect(rect, Color::Background);
        }

        need_draw = false;

        return true;
    }

    return false;
}


void Item::SetShown(bool show)
{
    is_shown = show;

    need_draw = true;

    if (!is_shown)
    {
        if (Page::Current() == parent)
        {
            Draw();
        }
    }
}


ButtonToggle *Item::ToButtonToggle()
{
    return (type == TypeItem::ButtonToggle) ? (ButtonToggle *)this : nullptr;
}


ButtonPress *Item::ToButtonPress()
{
    return (type == TypeItem::ButtonPress) ? (ButtonPress *)this : nullptr;
}


Choice *Item::ToChoice()
{
    return (type == TypeItem::Choice) ? (Choice *)this : nullptr;
}


Label::Label(bool append, pchar _textRU, pchar _textEN, const Rect &_rect, Font::E _font, void (*_funcOnPress)(Item *, bool),
    const Color &_colorText, const Color &_colorBack, bool _h_aligned, bool _v_aligned) :
    Item(TypeItem::Label, _rect, _funcOnPress, append),
    font(_font), h_aligned(_h_aligned), v_aligned(_v_aligned), colorText(_colorText),
    colorBack(_colorBack.value == Color::Count.value ? Color::Background : _colorBack)
{
    std::strcpy(text[0], _textRU);
    std::strcpy(text[1], _textEN);
}


Label &Label::operator=(const Label &rhs)
{
    Item *item = (Item *)this;
    Item *item_rhs = (Item *)&rhs;

    *item = *item_rhs;

    std::strcpy(text[0], rhs.text[0]);
    std::strcpy(text[1], rhs.text[1]);
    font = rhs.font;
    h_aligned = rhs.h_aligned;
    v_aligned = rhs.v_aligned;
    colorText = rhs.colorText;
    colorBack = rhs.colorBack;

    return *this;
}


pchar Label::Text() const
{
    return text[set.lang];
}


bool Label::Draw()
{
    if (need_draw)
    {
        if (IsShown())
        {
            Nextion::DrawString(rect, font, colorText, colorBack, Text(), h_aligned, v_aligned);
        }
        else
        {
            Nextion::FillRect(rect, colorBack);
        }

        need_draw = false;

        return true;
    }

    return false;
}


bool Choice::Draw()
{
    if (need_draw)
    {
        need_draw = false;

        button.Refresh();
        label.Refresh();

        return true;
    }

    return false;
}


void Choice::SetParent(Page *page)
{
    Item::SetParent(page);
    button.SetParent(page);
}


void Choice::SetShown(bool show)
{
    is_shown = show;

    button.SetShown(show);
    label.SetShown(show);

    if (show)
    {
        SetTextValue();

        button.Refresh();
        need_draw = true;
    }
}


void Label::SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN])
{
    if (std::strcmp(_textRU, text[Lang::RU]) != 0)
    {
        std::strcpy(text[Lang::RU], _textRU);

        need_draw = true;
    }

    if (std::strcmp(_textEN, text[Lang::EN]) != 0)
    {
        std::strcpy(text[Lang::EN], _textEN);

        need_draw = true;
    }
}


int Item::GetCoordX(int col)
{
    int dx = 10;

    return dx + (ButtonCommon::WIDTH_MENU + dx) * col;
}


int Item::GetCoordY(int row)
{
    int dy = 10;

    return dy + (ButtonCommon::HEIGHT_MENU + dy) * row;
}
