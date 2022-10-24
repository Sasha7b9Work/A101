// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageMain.h"
#include "Hardware/HAL/HAL_PIO.h"


namespace PageMain
{
    Button *GetButton(int index)
    {
        static Button *buttons[7] = { btn2mA, btn2mA, btn20mA, btn200mA, btn2A, btn20A, btn50A };

        return buttons[index];
    }

    static void Function(const Button *button, int action)
    {
        if (action == 1)
        {
            int index = button->GetIndex();

            if (index >= 0x01 && index <= 0x06)
            {
                for (int i = 1; i <= 6; i++)
                {
                    GetButton(i)->RemoveBacklight();
                }

                static int states[6][7] =
                {
                    {1, 0, 0, 0, 1, 1, 0},      // 2mA
                    {1, 0, 1, 1, 0, 1, 0},      // 20mA
                    {1, 1, 0, 1, 1, 0, 0},      // 200mA
                    {0, 0, 0, 1, 0, 0, 0},      // 2A
                    {1, 0, 1, 1, 0, 1, 0},      // 20A
                    {1, 1, 0, 1, 1, 0, 0}       // 50A
                };

                int range = index - 1;

                HAL_PIO::Write(PIN_US1, states[range][0] == 1); //-V525
                HAL_PIO::Write(PIN_US2, states[range][1] == 1);
                HAL_PIO::Write(PIN_US3, states[range][2] == 1);
                HAL_PIO::Write(PIN_US4, states[range][3] == 1);
                HAL_PIO::Write(PIN_US6, states[range][4] == 1);
                HAL_PIO::Write(PIN_US7, states[range][5] == 1);
                HAL_PIO::Write(PIN_US8, states[range][6] == 1);

                GetButton(index)->SetBacklight();
            }
        }
    }

    static void ApplyActionButton2mA(int action)
    {
        Function(btn2mA, action);
    }

    static Button button2mA(1, ApplyActionButton2mA);


    static void ApplyActionButton20mA(int action)
    {
        Function(btn20mA, action);
    }

    static Button button20mA(2, ApplyActionButton20mA);


    static void ApplyActionButton200mA(int action)
    {
        Function(btn200mA, action);
    }

    static Button button200mA(3, ApplyActionButton200mA);


    static void ApplyActionButton2A(int action)
    {
        Function(btn2A, action);
    }

    static Button button2A(4, ApplyActionButton2A);


    static void ApplyActionButton20A(int action)
    {
        Function(btn20A, action);
    }

    static Button button20A(5, ApplyActionButton20A);


    static void ApplyActionButton50A(int action)
    {
        Function(btn50A, action);
    }

    static Button button50A(6, ApplyActionButton50A);


    Button *btn2mA = &button2mA;
    Button *btn20mA = &button20mA;
    Button *btn200mA = &button200mA;
    Button *btn2A = &button2A;
    Button *btn20A = &button20A;
    Button *btn50A = &button50A;
}

