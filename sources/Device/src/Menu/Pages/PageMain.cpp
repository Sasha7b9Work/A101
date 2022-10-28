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

        return -1;
    }

    static void Function(const Button *button, int)
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

    static void ApplyActionButton2mA(int state)
    {
        Function(btn2mA, state);
    }

    static Button button2mA("button0", "2 mA", false, ApplyActionButton2mA);


    static void ApplyActionButton20mA(int state)
    {
        Function(btn20mA, state);
    }

    static Button button20mA("button1", "20 mA", false, ApplyActionButton20mA);


    static void ApplyActionButton200mA(int state)
    {
        Function(btn200mA, state);
    }

    static Button button200mA("button2", "200 mA", false, ApplyActionButton200mA);


    static void ApplyActionButton2A(int state)
    {
        Function(btn2A, state);
    }

    static Button button2A("button3", "2 A", true, ApplyActionButton2A);


    static void ApplyActionButton20A(int state)
    {
        Function(btn20A, state);
    }

    static Button button20A("button4", "20 A", false, ApplyActionButton20A);


    static void ApplyActionButton50A(int state)
    {
        Function(btn50A, state);
    }

    static Button button50A("button5", "50 A", false, ApplyActionButton50A);


    Button *btn2mA = &button2mA;
    Button *btn20mA = &button20mA;
    Button *btn200mA = &button200mA;
    Button *btn2A = &button2A;
    Button *btn20A = &button20A;
    Button *btn50A = &button50A;

    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

