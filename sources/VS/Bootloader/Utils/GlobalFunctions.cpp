// 2025/11/12 14:48:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/GlobalFunctions.h"


void GF::SendCommandEvent(wxWindow *object, int type_event, int id, int int_value)
{
    wxCommandEvent evt(type_event, id);
    evt.SetEventObject(object);
    evt.SetInt(int_value);
    wxPostEvent(object->GetEventHandler(), evt);
}
