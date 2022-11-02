// 2022/11/02 09:17:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Dialogs/Dialog.h"


class DialogNumberComPort : public Dialog
{
public:

    DialogNumberComPort();

    static int NumComPort() { return num_port; };

private:

    virtual void ApplyParameters();
    virtual void CancelParameters();

    wxTextCtrl *txtNumberComPort = nullptr;

    static int num_port;
};
