// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/DiagramFFT.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Nextion/Display.h"


namespace PageGraph
{
    static void FuncOnEnter()
    {

    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }


    static ButtonToggle btnWave("Сигнал", "Signal", Font::_1, { 8, 5, 160, 80 }, [](Item *, bool)
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static ButtonToggle btnSpectr("Спектр", "Spectr", Font::_1, { 172, 5, 160, 80 }, [](Item *, bool)
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });

    static ButtonPress btnBack("Назад", "Back", Font::_1, { 635, 5, 160, 80 }, [](Item *, bool)
        {
            PageMain::self->SetAsCurrent();
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

    void EnableDebug()
    {
//        pageTwo.SetItem(4, &btnDebug);
    }
}
