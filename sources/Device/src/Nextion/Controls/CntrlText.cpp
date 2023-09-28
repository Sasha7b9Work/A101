// 2023/09/22 16:39:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Controls/CntrlText.h"
#include "Nextion/Nextion.h"
#include <cstring>


void CntrlText::SetLabel(pchar label)
{
    if (name[0])
    {
        if (std::strcmp(label, prev_label) != 0)
        {
            std::strcpy(prev_label, label);

            Nextion::SendCommandFormat("%s.txt=\"%s\"", name, label);

            LOG_WRITE_TRACE("Set label %s", label);
        }
    }
}
