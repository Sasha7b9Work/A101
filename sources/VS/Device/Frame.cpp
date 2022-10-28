// 2022/10/28 22:55:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"


Frame *Frame::self = nullptr;


Frame::Frame(const wxString &title)
    :wxFrame((wxFrame *)NULL, wxID_ANY, title)
{

}
