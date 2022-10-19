// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"


namespace DiagramInput
{
    static BufferADC data;
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


void DiagramInput::Draw()
{

}
