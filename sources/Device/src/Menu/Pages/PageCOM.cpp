// 2024/09/10 15:24:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItems.h"


namespace PageCOM
{
    static pchar names_baudrates[] =
    {
        "9600",   "9600",
        "115200", "115200",
        nullptr
    };

    static Choice chBaudrate("Скорость", "Baudrate", names_baudrates, 0, 0, [](Item *, bool)
    {

    });

    static pchar names_parity[] =
    {
        "Да",  "Yes",
        "Нет", "No",
        nullptr
    };

    static Choice chParity("Четность", "Parity", names_parity, 0, 1, [](Item *, bool)
    {

    });

    static pchar names_stop_bits[] =
    {
        "1",   "1",
        "1.5", "1.5",
        "2",   "2",
        nullptr
    };

    static Choice chStopBits("Стоп-биты", "Stop-bits", names_stop_bits, 0, 2, [](Item *, bool)
    {

    });

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
        }
    });

    static Item *items[] =
    {
        &chBaudrate,
        &chParity,
        &chStopBits,
        &btnBack,
        nullptr
    };

    static Page pageCOM(items, nullptr, nullptr);

    Page *self = &pageCOM;
}
