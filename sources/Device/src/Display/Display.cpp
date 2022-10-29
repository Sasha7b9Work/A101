// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Display/DiagramInput.h"
#include "Display/DInterface.h"
#include "Display/Indicator.h"
#include "Display/Painter.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


void Display::Init()
{
    Indicator::AutoSize();
}


void Display::Update()
{
    DiagramInput::Draw();
}


void Display::SetData(const BufferADC &data)
{
    DiagramInput::SetData(data);
}
