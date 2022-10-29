// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"


namespace PageTwo
{
    static void ApplyActionButtonInput(Button *button)
    {
        button->ToggleHighlight();

        DiagramInput::Enable(button->IsHightlight());
    }

    static Button buttonInput("button0", "Input", false, ApplyActionButtonInput);


    static void ApplyActionButtonFFT(Button *button)
    {
        button->ToggleHighlight();

        DiagramFFT::Enable(button->IsHightlight());
    }

    static Button buttonFFT("button1", "FFT", false, ApplyActionButtonFFT);


    static void ApplyActionButtonZero(Button *button)
    {
        button->ToggleHighlight();

        if (button->IsHightlight())
        {

        }
        else
        {

        }
    }

    static Button buttonZero("button2", "Zero", false, ApplyActionButtonZero);


    static void ApplyActionButton3(Button *)
    {

    }

    static Button button3("button3", "", false, ApplyActionButton3);


    static void ApplyActionButton4(Button *)
    {

    }

    static Button button4("button4", "", false, ApplyActionButton4);


    static void ApplyActionButtonDebug(Button *)
    {

    }

    static Button buttonDebug("button5", "Debug", false, ApplyActionButtonDebug);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnZero = &buttonZero;

    static Page pageTwo(&buttonInput, &buttonFFT, &buttonZero, &button3, &button4, &buttonDebug);

    Page *self = &pageTwo;
}
