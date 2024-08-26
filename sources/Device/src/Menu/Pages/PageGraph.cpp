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


    static ButtonOld btnWave("btnSignal", "3WP", []()
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static ButtonOld btnSpectr("btnSpectr", "3SP", []()
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });

    static ButtonOld btnBack("btnBackToMain", "3B0", []()
        {
            PageMain::self->SetAsCurrent();
        });

    static ButtonOld btnDebug("", "debug", []()
        {
            PageDebug::self->SetAsCurrent();
        });


    static ButtonCommon *buttons[] =
    {
        &btnWave,
        &btnSpectr,
        &btnBack,
        nullptr
    };

    static Page pageTwo(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageTwo;

    void EnableDebug()
    {
        pageTwo.SetButton(4, &btnDebug);
    }
}
