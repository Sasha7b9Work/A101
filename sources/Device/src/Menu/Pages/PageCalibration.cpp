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

    static Button buttonZero("button2", "Zero", false, FuncPress_Zero);


    static void FuncPress_1(Button *)
    {

    }

    static Button button1("button1", "", false, FuncPress_1);


    static void FuncPress_2(Button *)
    {

    }

    static Button button2("button2", "", false, FuncPress_2);


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


    static Page pageCalibration(&buttonZero, &button1, &button2, &button3, &button4, &button5);

    Button *btnZero = &buttonZero;

    Page *self = &pageCalibration;
}
