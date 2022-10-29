// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageMain.h"
#include "Hardware/HAL/HAL_PIO.h"


namespace PageMain
{
    static int GetIndex(const Button *button)
    {
        for (int i = 0; i < 6; i++)
        {
            if (PageMain::self->GetButton(i) == button)
            {
                return i;
            }
        }

        LOG_WRITE("Invalid index page");

        return 0;
    }

    static void Function(const Button *button)
    {
        int index = GetIndex(button);

        static int states[6][7] =
        {
            {1, 0, 0, 0, 1, 1, 0},      // 2mA
            {1, 0, 1, 1, 0, 1, 0},      // 20mA
            {1, 1, 0, 1, 1, 0, 0},      // 200mA
            {0, 0, 0, 1, 0, 0, 0},      // 2A
            {1, 0, 1, 1, 0, 1, 0},      // 20A
            {1, 1, 0, 1, 1, 0, 0},      // 50A
        };

        HAL_PIO::Write(PIN_US1, states[index][0] == 1); //-V525
        HAL_PIO::Write(PIN_US2, states[index][1] == 1);
        HAL_PIO::Write(PIN_US3, states[index][2] == 1);
        HAL_PIO::Write(PIN_US4, states[index][3] == 1);
        HAL_PIO::Write(PIN_US6, states[index][4] == 1);
        HAL_PIO::Write(PIN_US7, states[index][5] == 1);
        HAL_PIO::Write(PIN_US8, states[index][6] == 1);

        for (int i = 0; i < 6; i++)
        {
            PageMain::self->GetButton(i)->SetHighlight(index == i);
        }
    }

    static void FuncPress_2mA(Button *btn)
    {
        Function(btn);
    }

    static Button button2mA("button0", "2 mA", false, FuncPress_2mA);


    static void FuncPress_20mA(Button *btn)
    {
        Function(btn);
    }

    static Button button20mA("button1", "20 mA", false, FuncPress_20mA);


    static void FuncPress_200mA(Button *btn)
    {
        Function(btn);
    }

    static Button button200mA("button2", "200 mA", false, FuncPress_200mA);


    static void FuncPress_2A(Button *btn)
    {
        Function(btn);
    }

    static Button button2A("button3", "2 A", true, FuncPress_2A);


    static void FuncPress_20A(Button *btn)
    {
        Function(btn);
    }

    static Button button20A("button4", "20 A", false, FuncPress_20A);


    static void FuncPress_50A(Button *btn)
    {
        Function(btn);
    }

    static Button button50A("button5", "50 A", false, FuncPress_50A);


    Button *btn2mA = &button2mA;
    Button *btn20mA = &button20mA;
    Button *btn200mA = &button200mA;
    Button *btn2A = &button2A;
    Button *btn20A = &button20A;
    Button *btn50A = &button50A;

    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

