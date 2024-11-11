// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Nextion/Display.h"


namespace PageGraph
{
    static void FuncOnEnter()
    {
        DiagramInput::Reset(true);
    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }

    static ButtonMenuPress btnTypeGraph("Сигнал", "Signal", 0, 0, [](Item *item, bool press)
    {
        if (!press)
        {
            set.type_signal.Increase();

            static const pchar titles[2][Lang::Count] =
            {
                {"Сигнал", "Signal"},
                {"FFT",    "FFT"}
            };

            item->ToButtonPress()->SetText(titles[set.type_signal.value][Lang::RU], titles[set.type_signal.value][Lang::EN]);

            FuncOnEnter();
        }
    });

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

    static Item *items[] =
    {
        &btnTypeGraph,
        &btnBack,
        nullptr
    };

    static Page pageTwo(items, FuncOnEnter, FuncDraw);

    Page *self = &pageTwo;
}
