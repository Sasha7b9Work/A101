// 2024/08/21 21:30:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Sizer.h"


void Sizer::Enable(bool enable)
{
    for (int i = 0; i < GetItemCount(); i++)
    {
        wxWindow *window = GetItem(i)->GetWindow();

        if (window)
        {
            window->Enable(enable);
        }
    }
}
