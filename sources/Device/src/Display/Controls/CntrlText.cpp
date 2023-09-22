// 2023/09/22 16:39:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Controls/CntrlText.h"
#include "Nextion/Nextion.h"


void CntrlText::SetLabel(pchar label)
{
    Nextion::Text::SetText(name, label);
}
