// 2023/09/22 16:39:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Controls/CntrlText.h"
#include "Nextion/Nextion.h"
#include <cstring>


void CntrlText::SetLabel(pchar label, bool if_different)
{
    if (name[0])
    {
        if (std::strcmp(label, prev_label) != 0 || !if_different)
        {
            std::strcpy(prev_label, label);

            Nextion::Text::SetLabel(name, label);
        }
    }
}


void CntrlText::SetVisible(bool visible)
{
    Nextion::Text::SetVisible(name, visible);
}


void CntrlText::Flash()
{
    SetLabel(prev_label, false);
}
