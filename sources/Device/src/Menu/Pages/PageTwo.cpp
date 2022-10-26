// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"


namespace PageTwo
{
    static void ApplyActionButtonInput(int)
    {

    }

    static Button buttonInput(1, ApplyActionButtonInput);


    static void ApplyActionButtonFFT(int)
    {

    }

    static Button buttonFFT(2, ApplyActionButtonFFT);


    static void ApplyActionButtonZero(int)
    {

    }

    static Button buttonZero(3, ApplyActionButtonZero);


    static void ApplyActionButtonEmpty2(int)
    {
    }

    static Button buttonEmpty2(4, ApplyActionButtonEmpty2);


    static void ApplyActionButtonEmpty3(int)
    {
    }

    static Button buttonEmpty3(5, ApplyActionButtonEmpty3);


    static void ApplyActionButtonEmpty4(int)
    {
    }

    static Button buttonEmpty4(6, ApplyActionButtonEmpty4);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnZero = &buttonZero;
    Button *btnEmpty2 = &buttonEmpty2;
    Button *btnEmpty3 = &buttonEmpty3;
    Button *btnEmpty4 = &buttonEmpty4;
}
