// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/dialog.h>
#pragma warning(pop)


class Dialog;


class SpinControl : public wxSpinCtrl
{
public:
    SpinControl(wxWindow *window, wxWindowID id, const wxPoint &position, const wxSize &size, int min, int max, const int initial,
        wxDialog *dlg, wxEventFunction handler, const wxString &label, Dialog *dialog);

private:
    Dialog *dialog = nullptr;

    int prevValue = 0;

    void OnKeyUp(wxKeyEvent &);

    char buffer[100];
};
