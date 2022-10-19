// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"


using namespace Primitives;


namespace DiagramInput
{
    static BufferADC data;

    static const int width = 800;
    static const int height = 200;

    static void Clear();
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


void DiagramInput::Draw()
{
    Clear();

    Rectangle(width, height).Draw(0, 0, Color::White);
}


void DiagramInput::Clear()
{
    Rectangle(width, height).Fill(0, 0, Color::Black);
}
