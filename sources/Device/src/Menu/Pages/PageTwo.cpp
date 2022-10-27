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

    static Button buttonInput(7, "button1", "Input", ApplyActionButtonInput);


    static void ApplyActionButtonFFT(int)
    {

    }

    static Button buttonFFT(8, "button2", "FFT", ApplyActionButtonFFT);


    static void ApplyActionButtonZero(int)
    {

    }

    static Button buttonZero(9, "button3", "Zero", ApplyActionButtonZero);


    static void ApplyActionButtonEmpty2(int)
    {
    }

    static Button buttonEmpty2(10, "button4", "", ApplyActionButtonEmpty2);


    static void ApplyActionButtonEmpty3(int)
    {
    }

    static Button buttonEmpty3(11, "button5", "", ApplyActionButtonEmpty3);


    static void ApplyActionButtonEmpty4(int)
    {
    }

    static Button buttonEmpty4(12, "button6", "", ApplyActionButtonEmpty4);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnZero = &buttonZero;
    Button *btnEmpty2 = &buttonEmpty2;
    Button *btnEmpty3 = &buttonEmpty3;
    Button *btnEmpty4 = &buttonEmpty4;

    static Button *buttons[6] = { btnInput, btnFFT, btnZero, btnEmpty2, btnEmpty3, btnEmpty4 };

    void *self = (void *)buttons;
}
