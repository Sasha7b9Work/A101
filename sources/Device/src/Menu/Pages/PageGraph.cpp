// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Display/Indicator.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator.h"


namespace PageGraph
{
    static void FuncOnEnable(bool)
    {

    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }


    static Button btnWave("bt0", "3WP", [](Button *)
        {
            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button btnSpectr("bt1", "3SP", [](Button *)
        {
            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });

    static Button btnBack("bt6", "3B0", [](Button *) {});

    static Button btnDebug("", "debug", [](Button *)
        {
            PageDebug::self->SetAsCurrent();
        });


    static Button btnCalibration("", "calib", [](Button *)
        {
            Calibrator::ExecuteCalibration();
        });


    static Button *buttons[] =
    {
        &btnWave,
        &btnSpectr,
        &btnBack,
        nullptr
    };

    static Page pageTwo(buttons, FuncOnEnable, FuncDraw);

    Page *self = &pageTwo;

    void EnableDebug()
    {
        pageTwo.SetButton(4, &btnDebug);
    }


    void EnableCalibration()
    {
        pageTwo.SetButton(5, &btnCalibration);
    }
}
