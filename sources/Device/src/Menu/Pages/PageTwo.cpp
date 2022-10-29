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


    static void FuncPress_2(Button *)
    {

    }

    static Button button2("button2", "", false, FuncPress_2);


    static void FuncPress_3(Button *)
    {

    }

    static Button button3("button3", "", false, FuncPress_3);


    static void FuncPress_Debug(Button *)
    {

    }

    static Button buttonDebug("button4", "Debug", false, FuncPress_Debug);


    static void FuncPress_Calibration(Button *)
    {

    }

    static Button buttonCalibration("button5", "Calibr", false, FuncPress_Calibration);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;

    static Page pageTwo(&buttonInput, &buttonFFT, &button2, &button3, &buttonDebug, &buttonCalibration);

    Page *self = &pageTwo;
}
