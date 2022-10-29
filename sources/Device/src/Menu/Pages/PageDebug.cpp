// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageDebug.h"


namespace PageDebug
{
    static void FuncPress0(Button *)
    {

    }

    static Button button0("button0", "", false, FuncPress0);


    static void FuncPress1(Button *)
    {

    }

    static Button button1("button1", "", false, FuncPress1);


    static void FuncPress2(Button *)
    {

    }

    static Button button2("button2", "", false, FuncPress2);


    static void FuncPress3(Button *)
    {

    }

    static Button button3("button3", "", false, FuncPress3);


    static void FuncPress4(Button *)
    {

    }

    static Button button4("button4", "", false, FuncPress4);


    static void FuncPress5(Button *)
    {

    }

    static Button button5("button5", "", false, FuncPress5);


    static Page pageDebug(&button0, &button1, &button2, &button3, &button4, &button5);

    Page *self = &pageDebug;
}
