// 2022/11/02 09:17:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Dialogs/DialogNumberComPort.h"


int DialogNumberComPort::num_port = 4;


DialogNumberComPort::DialogNumberComPort() : Dialog("Номер ком-порта")
{
    txtNumberComPort = new wxTextCtrl(this, wxID_ANY, wxString::Format("%d", num_port));

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(txtNumberComPort, wxALIGN_CENTER_HORIZONTAL);

    SetBoxSizer(sizer, { 200, 50 });
}


void DialogNumberComPort::ApplyParameters()
{
    int value = 0;

    if (txtNumberComPort->GetValue().ToInt(&value))
    {
        num_port = value;
    }
}


void DialogNumberComPort::CancelParameters()
{

}
