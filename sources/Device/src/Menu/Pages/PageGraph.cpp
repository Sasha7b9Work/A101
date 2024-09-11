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
        DiagramInput::Reset();
    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }

    static int type_signal = 0;     // Сигнал, спектр, оба

    static ButtonMenuPress btnWave("Сигнал", "Signal", 0, 0, [](Item *item, bool press)
    {
        if (!press)
        {
            type_signal++;

            if (type_signal > 2)
            {
                type_signal = 0;
            }

            static const pchar titles[3][Lang::Count] =
            {
                {"Сигнал", "Signal"},
                {"FFT",    "FFT"},
                {"Оба",    "Together"}
            };

            item->ToButtonPress()->SetText(titles[type_signal][Lang::RU], titles[type_signal][Lang::EN]);
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
        &btnWave,
        &btnBack,
        nullptr
    };

    static Page pageTwo(items, FuncOnEnter, FuncDraw);

    Page *self = &pageTwo;
}
