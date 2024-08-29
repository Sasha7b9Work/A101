// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include "Hardware/Timer.h"
#include "Utils/String.h"
#include <cstring>
#include <cstdio>


using namespace std;


Page *Page::current = PageMain::self;


namespace NS_ITEMS
{
    static const int MAX_ITEMS = 1024;                  // Здесь хранятся все созданные кнопки (со всех страниц)
    static Item *items[MAX_ITEMS];
    static int num_items = 0;                         // Столько создано кнопок в данный момент

    static void AppendNewItem(Item *item)
    {
        items[num_items] = item;

        if (num_items++ >= MAX_ITEMS)
        {
            LOG_ERROR_TRACE("The items buffer is full");
        }
    }

    static Item *GetItem(int x, int y)
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
}


void Item::SetAllInactive()
{
    for (int i = 0; i < NS_ITEMS::num_items; i++)
    {
        NS_ITEMS::items[i]->SetShown(false);
    }
}


void Item::Press()
{
    is_pressed = true;

    Draw();

    funcOnPress();
}


void Item::Release()
{
    if (IsWithoutFixation())
    {
        is_pressed = false;

        return;
    }
}


Item::Item(TypeItem::E _type, int _x, int _y, int _h, int _w, void (*_funcOnPress)()) :
    type(_type), rect{ (int16)_x, (int16)_y, (int16)_w, (int16)_h }, funcOnPress(_funcOnPress)
{
    NS_ITEMS::AppendNewItem(this);
}


ButtonCommon::ButtonCommon(TypeItem::E _type, pchar title_ru, pchar title_en, Font::E _f, int _x, int _y, int _w, int _h, void (*_funcOnPress)()) :
    Item(_type, _x, _y, _h, _w, _funcOnPress),
    font(_f)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;
}


void Item::OnEventPress(int x, int y)
{
    Item *item = NS_ITEMS::GetItem(x, y);

    if (item)
    {
        item->Press();
    }
}


void Item::OnEventRelease(int x, int y)
{
    Item *item = NS_ITEMS::GetItem(x, y);

    if (item)
    {
        item->Release();
    }
}


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    PageMain::Init();
}


void ButtonOld::Press()
{
    funcOnPress();
}


void ButtonOld::SetText(pchar txt) const
{
    if (IsSoftware())
    {

    }
    else
    {
        Nextion::Button::SetText(name, txt);
    }
}


void ButtonOld::SetVisible(bool visible)
{
    if (IsSoftware())
    {

    }
    else
    {
        Nextion::SetVisible(name, visible);

        is_visible = visible;
    }
}


void ButtonOld::SetValue(int _value)
{
    value = _value;

    if (IsSoftware())
    {

    }
    else
    {
        Nextion::SendCommandFormat("%s.val=%d", name, value);
    }
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
    ButtonCommon::SetAllInactive();

    current = this;

    Timer::Delay(50);           // Эта задержка нужна для того, чтобы дисплей успел переключиться на новую страницу.
                                // Иначе сообщения элементам управления будут посылаться на старую страницу

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->SetShown(true);
    }

    current->funcOnEnter();
}

ButtonCommon *Page::GetButton(pchar signal)
{
    Item **item = &items[0];

    while (*item)
    {
        ButtonOld *button = (*item)->ToButtonOld();

        if (button)
        {
            if (std::strcmp(button->Signal(), signal) == 0)
            {
                return button;
            }
        }

        item++;
    }

    return nullptr;
}


void Page::Draw()
{
    funcOnDraw();

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->Draw();
    }
}


void ButtonOld::Draw()
{
    if (!IsSoftware())
    {
        return;
    }

    int width = 150;
    int height = 50;

    Nextion::DrawLine(x, y, x + width, y, Color::White);
    Nextion::DrawLine(x, y + 1, x + width, y + 1, Color::White);

    Nextion::DrawLine(x, y + height, x + width, y + height);
    Nextion::DrawLine(x, y + height + 1, x + width, y + height + 1);

    Nextion::DrawLine(x, y, x, y + height);
    Nextion::DrawLine(x + 1, y, x + 1, y + height);

    Nextion::DrawLine(x + width, y, x + width, y + height);
    Nextion::DrawLine(x + width + 1, y, x + width + 1, y + height);

    Nextion::DrawString(x + 2, y + height / 2 - 16, width - 4, height / 2, 2, Color::White, Color::Background, Text(), 1);
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


bool ButtonOld::IsSoftware() const
{
    return x >= 0;
}


Button::Button(pchar title_ru, pchar title_en, Font::E _f, int _x, int _y, int _w, int _h, void (*_funcOnPress)()) :
    ButtonCommon(TypeItem::ItemButton, title_ru, title_en, _f, _x, _y, _w, _h, _funcOnPress)
{
}


void ButtonCommon::SetValue(int value)
{
    bool new_pressed = (value != 0);

    if (new_pressed != is_pressed)
    {
        is_pressed = new_pressed;

        Draw();
    }
}


pchar Button::Signal() const
{
    return "";
}


void Button::Draw()
{
    int x = rect.x;
    int y = rect.y;
    int width = rect.width;
    int height = rect.height;

    if (IsShown())
    {
        Nextion::DrawRect(x, y, width - 1, height - 1, Color::White);
        Nextion::DrawRect(x + 1, y + 1, width - 3, height - 3, Color::White);
        Nextion::DrawRect(x + 2, y + 2, width - 5, height - 5, Color::White);

        Nextion::DrawString(x + 3, y + 3, width - 7, height - 7, font,
            Color::White,
            is_pressed ? Color::ButtonPress : Color::Background,
            title[set.lang], 1, 1);
    }
    else
    {
        Nextion::FillRect(x, y, width, height, Color::Background);
    }
}


bool Item::IsShown() const
{
    return (Page::Current() == parent) && is_shown;
}


void Item::SetShown(bool show)
{
    is_shown = show;

    Draw();
}


ButtonRange *Item::ToButtonRange()
{
    return (type == TypeItem::ItemButtonToggle) ? (ButtonRange *)this : nullptr;
}


ButtonOld *Item::ToButtonOld()
{
    return (type == TypeItem::ItemButtonOld) ? (ButtonOld *)this : nullptr;
}
