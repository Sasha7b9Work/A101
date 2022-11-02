// 2022/10/28 22:55:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    static Frame *self;

private:

    wxTimer timer;

    void SetSizeAndPosition();

    void CreateMenu();

    void OnTimerEvent(wxTimerEvent &);
    void OnQuitEvent(wxCommandEvent &);
    void OnNumberPortEvent(wxCommandEvent &);
};
