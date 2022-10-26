// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageTwo.h"


namespace PageTwo
{
    static void ApplyActionButtonInput()
    {
    }

    static Button buttonInput(1, ApplyActionButtonInput);


    static void ApplyActionButtonFFT()
    {
    }

    static Button buttonFFT(2, ApplyActionButtonFFT);


    static void ApplyActionButtonEmpty1()
    {
    }

    static Button buttonEmpty1(3, ApplyActionButtonEmpty1);


    static void ApplyActionButtonEmpty2()
    {
    }

    static Button buttonEmpty2(4, ApplyActionButtonEmpty2);


    static void ApplyActionButtonEmpty3()
    {
    }

    static Button buttonEmpty3(5, ApplyActionButtonEmpty3);


    static void ApplyActionButtonEmpty4()
    {
    }

    static Button buttonEmpty4(6, ApplyActionButtonEmpty4);


    Button *btnInput = &buttonInput;
    Button *btnFFT = &buttonFFT;
    Button *btnEmpty1 = &buttonEmpty1;
    Button *btnEmpty2 = &buttonEmpty2;
    Button *btnEmpty3 = &buttonEmpty3;
    Button *btnEmpty4 = &buttonEmpty4;
}
