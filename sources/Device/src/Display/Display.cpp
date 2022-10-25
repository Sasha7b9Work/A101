// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Display/DiagramInput.h"
#include "Display/DInterface.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


void Display::SetDC(float value)
{
    Interface::SendCommandFormat("textDC.txt=\"%.4f V\"", value);
    Interface::SendCommandFormat("textDCsmall.txt=\"%.4f V\"", value);
}


void Display::SetAC(float value)
{
    Interface::SendCommandFormat("textAC.txt=\"%.4f V\"", value);
    Interface::SendCommandFormat("textACsmall.txt=\"%.4f V\"", value);
}


void Display::Update()
{
    DiagramInput::Draw();
}


void Display::SetData(const BufferADC &data)
{
    DiagramInput::SetData(data);
}
