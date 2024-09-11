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


    static ButtonToggle btnWave("Сигнал", "Signal", Font::_1, { 8, 5, 160, 80 }, [](Item *, bool)
    {
    });


    static ButtonToggle btnSpectr("Спектр", "Spectr", Font::_1, { 172, 5, 160, 80 }, [](Item *, bool)
    {
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
        &btnSpectr,
        &btnBack,
        nullptr
    };

    static Page pageTwo(items, FuncOnEnter, FuncDraw);

    Page *self = &pageTwo;
}
