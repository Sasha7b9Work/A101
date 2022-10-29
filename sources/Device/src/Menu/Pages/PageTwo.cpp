// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageCalibration.h"
#include "Display/Indicator.h"


namespace PageTwo
{
    static Button buttonInput("button0", "Input", false, [](Button *button)
        {
            button->ToggleHighlight();

            DiagramInput::Enable(button->IsHightlight());

            Indicator::AutoSize();
        });


    static Button buttonFFT("button1", "FFT", false, [](Button *button)
        {
            button->ToggleHighlight();

            DiagramFFT::Enable(button->IsHightlight());

            Indicator::AutoSize();
        });


    static Button button2("button2", "", false, [](Button *) {});


    static Button button3("button3", "", false, [](Button *) {});


    static Button buttonDebug("button4", "Debug", false, [](Button *)
        {
            PageDebug::self->SetAsCurrent();
        });


    static Button buttonCalibration("button5", "Calibr", false, [](Button *)
        {
            PageCalibration::self->SetAsCurrent();
        });


    static Page pageTwo(&buttonInput, &buttonFFT, &button2, &button3, &buttonDebug, &buttonCalibration);

    Page *self = &pageTwo;
}
