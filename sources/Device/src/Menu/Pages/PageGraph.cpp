// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Display/Display.h"


namespace PageGraph
{
    static void FuncOnEnter()
    {

    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }


    static Button btnWave("bt0", "3WP", []()
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button btnSpectr("bt1", "3SP", []()
        {
            DiagramInput::Draw();

            DiagramFFT::Draw();
        });

    static Button btnBack("bt6", "3B0", []()
        {
            PageMain::self->SetAsCurrent();
        });

    static Button btnDebug("", "debug", []()
        {
            PageDebug::self->SetAsCurrent();
        });


    static Button *buttons[] =
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
