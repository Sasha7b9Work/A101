// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageCalibration.h"


namespace PageCalibration
{
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

    static Button buttonZero("button0", "Zero", false, FuncPress_Zero);


    static void FuncPress_0A(Button *)
    {

    }

    static Button button0A("button1", "0 A", false, FuncPress_0A);


    static void FuncPress_2A(Button *)
    {

    }

    static Button button2A("button2", "2 A", false, FuncPress_2A);


    static void FuncPress_3(Button *)
    {

    }

    static Button button3("button3", "", false, FuncPress_3);


    static void FuncPress_4(Button *)
    {

    }

    static Button button4("button4", "", false, FuncPress_4);


    static void FuncPress_5(Button *)
    {

    }

    static Button button5("button5", "", false, FuncPress_5);


    static Page pageCalibration(&buttonZero, &button0A, &button2A, &button3, &button4, &button5);

    Page *self = &pageCalibration;
}
