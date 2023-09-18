// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Display/Indicator.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator.h"


namespace PageGraph
{
    static Button buttonInput("input", [](Button *)
        {
            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button buttonFFT("fft", [](Button *)
        {
            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button button2("button2", [](Button *) {});


    static Button button3("button3", [](Button *) {});


    static Button button4("button4", [](Button *) {});


    static Button button5("button5", [](Button *) {});


    static Button buttonDebug("debug", [](Button *)
        {
            PageDebug::self->SetAsCurrent();
        });


    static Button buttonCalibration("calib", [](Button *)
        {
            Calibrator::ExecuteCalibration();
        });


    static Page pageTwo(&buttonInput, &buttonFFT, &button2, &button3, &buttonDebug, &buttonCalibration);

    Page *self = &pageTwo;

    void EnableDebug()
    {
        pageTwo.SetButton(4, &buttonDebug);
    }


    void EnableCalibration()
    {
        pageTwo.SetButton(5, &buttonCalibration);
    }
}
