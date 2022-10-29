// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"


namespace PageTwo
{
    static void FuncPress_Input(Button *button)
    {
        button->ToggleHighlight();

        DiagramInput::Enable(button->IsHightlight());
    }

    static Button buttonInput("button0", "Input", false, FuncPress_Input);


    static void FuncPress_FFT(Button *button)
    {
        button->ToggleHighlight();

        DiagramFFT::Enable(button->IsHightlight());
    }

    static Button buttonFFT("button1", "FFT", false, FuncPress_FFT);


    static void FuncPress_Zero(Button *button)
    {
        button->ToggleHighlight();

        if (button->IsHightlight())
        {

        }
        else
        {

        }
    }

    static Button buttonZero("button2", "Zero", false, FuncPress_Zero);


    static void FuncPress_3(Button *)
    {

    }

    static Button button3("button3", "", false, FuncPress_3);


    static void FuncPress_4(Button *)
    {

    }

    static Button button4("button4", "", false, FuncPress_4);


    static void FuncPress_Debug(Button *)
    {

    }

    static Button buttonDebug("button5", "Debug", false, FuncPress_Debug);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnZero = &buttonZero;

    static Page pageTwo(&buttonInput, &buttonFFT, &buttonZero, &button3, &button4, &buttonDebug);

    Page *self = &pageTwo;
}
