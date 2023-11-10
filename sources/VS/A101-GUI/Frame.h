// 2022/10/28 22:55:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "wx_includes.h"


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    static Frame *self;

private:

    wxTimer timer;

    void SetSizeAndPosition();

    void OnTimer(wxTimerEvent &);
    void OnQuit(wxCommandEvent &);
    void OnSCPI(wxCommandEvent &);
};
