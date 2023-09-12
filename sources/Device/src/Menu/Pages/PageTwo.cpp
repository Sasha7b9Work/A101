// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Display/Indicator.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator.h"


namespace PageTwo
{
    static Button buttonInput(false, [](Button *button)
        {
            button->ToggleHighlight();

            DiagramInput::Enable(button->IsHightlight());

            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button buttonFFT(false, [](Button *button)
        {
            button->ToggleHighlight();

            DiagramFFT::Enable(button->IsHightlight());

            Indicator::AutoSize();

            DiagramInput::Draw();

            DiagramFFT::Draw();
        });


    static Button button2(false, [](Button *) {});


    static Button button3(false, [](Button *) {});


    static Button button4(false, [](Button *) {});


    static Button button5(false, [](Button *) {});


    static Button buttonDebug(false, [](Button *)
        {
            PageDebug::self->SetAsCurrent();
        });


    static Button buttonCalibration(false, [](Button *)
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
