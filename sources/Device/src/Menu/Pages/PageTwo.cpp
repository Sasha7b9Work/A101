// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"
#include "Display/DiagramInput.h"


namespace PageTwo
{
    static void ApplyActionButtonInput(int action)
    {
        DiagramInput::Enable(action == 1);
    }

    static Button buttonInput(7, "button1", "Input", false, ApplyActionButtonInput);


    static void ApplyActionButtonFFT(int)
    {

    }

    static Button buttonFFT(8, "button2", "FFT", false, ApplyActionButtonFFT);


    static void ApplyActionButtonZero(int)
    {

    }

    static Button buttonZero(9, "button3", "Zero", false, ApplyActionButtonZero);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnZero = &buttonZero;

    static Page pageTwo(&buttonInput, &buttonFFT, &buttonZero, &Button::empty, &Button::empty, &Button::empty);

    Page *self = &pageTwo;
}
