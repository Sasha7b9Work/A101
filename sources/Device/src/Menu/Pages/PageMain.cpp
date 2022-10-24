// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageMain.h"


namespace PageMain
{
    static void ApplyActionButton2mA(int)
    {

    }

    static Button button2mA(ApplyActionButton2mA);


    static void ApplyActionButton20mA(int)
    {

    }

    static Button button20mA(ApplyActionButton20mA);


    static void ApplyActionButton200mA(int)
    {

    }

    static Button button200mA(ApplyActionButton200mA);


    static void ApplyActionButton2A(int)
    {

    }

    static Button button2A(ApplyActionButton2A);


    static void ApplyActionButton20A(int)
    {

    }

    static Button button20A(ApplyActionButton20A);


    static void ApplyActionButton50A(int)
    {

    }

    static Button button50A(ApplyActionButton50A);


    Button *btn2mA = &button2mA;
    Button *btn20mA = &button20mA;
    Button *btn200mA = &button200mA;
    Button *btn2A = &button2A;
    Button *btn20A = &button20A;
    Button *btn50A = &button50A;
}

