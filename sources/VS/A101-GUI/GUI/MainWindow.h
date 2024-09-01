// 2022/10/28 22:55:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "wx_includes.h"


class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString &title);

    static MainWindow *self;

private:

    wxTimer timer;

    void SetSizeAndPosition();

    void OnTimer(wxTimerEvent &);
    void OnClose(wxCloseEvent &);
    void OnQuit(wxCommandEvent &);
    void OnSCPI(wxCommandEvent &);
};
