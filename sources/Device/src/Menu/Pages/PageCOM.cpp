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

    static Choice chBaudrate("Скорость", "Baudrate", (uint8 *)&set.baudrate, names_baudrates, 0, 0);

    static pchar names_parity[] =
    {
        "Нет", "No",
        "Да",  "Yes",
        nullptr
    };

    static Choice chParity("Четность", "Parity", (uint8 *)&set.parity, names_parity, 0, 1);

    static pchar names_stop_bits[] =
    {
        "1",   "1",
        "1.5", "1.5",
        "2",   "2",
        nullptr
    };

    static Choice chStopBits("Стоп-биты", "Stop-bits", (uint8 *)&set.stop_bits, names_stop_bits, 0, 2);

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
            set.Save();
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
