// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Sizer.h"


class DiagramPool;


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    static Frame *self;

private:

    wxToolBar *toolBar = nullptr;

    Sizer *sizer_file = nullptr;
    Sizer *sizer_version = nullptr;
    wxBoxSizer *sizer_date = nullptr;
    wxStaticText *txtVersionValue = nullptr;
    wxStaticText *txtDateValue = nullptr;
    wxButton *btnSelectFile = nullptr;
    wxButton *btnUpdate = nullptr;

    void OnSize(wxSizeEvent &);

    void OnCloseWindow(wxCloseEvent &);

    void CreatePanel(wxWindow *);

    Sizer *CreateSizerFile(wxWindow *);

    Sizer *CreateSizerVersion(wxWindow *, const wxSize &);

    wxBoxSizer *CreateSizerDate(wxWindow *, const wxSize &);

    // Выбор файла
    void OnEventButtonSelectFile(wxCommandEvent &);

    void CreateFileFirmware(const wxString &);
};
