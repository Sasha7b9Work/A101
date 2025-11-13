// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls/Sizer.h"


class DiagramPool;


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    static Frame *self;

    // Вызываем, когда нужно закрыть порт
    void Disconnect();

private:

    void TryConnect();

    Sizer *sizer_file = nullptr;
    Sizer *sizer_version = nullptr;

    wxBoxSizer *sizer_date = nullptr;

    wxStaticText *txtVersionValue = nullptr;
    wxStaticText *txtDateValue = nullptr;
    wxStaticText *txtPort = nullptr;

    wxButton *btnSelectFile = nullptr;
    wxButton *btnUpgradeFirmware = nullptr;
    wxButton *btnUpdatePorts = nullptr;
    wxButton *btnConnect = nullptr;

    wxComboBox *cbComPorts = nullptr;

    void OnSize(wxSizeEvent &);

    void OnCloseWindow(wxCloseEvent &);

    void CreatePanel(wxWindow *);

    Sizer *CreateSizerFile(wxWindow *);

    Sizer *CreateSizerVersion(wxWindow *, const wxSize &);

    wxBoxSizer *CreateSizerDate(wxWindow *, const wxSize &);

    void OnEventButtonSelectFile(wxCommandEvent &);
    void OnEventButtonUpdatePorts(wxCommandEvent &);
    void OnEventButtonConnect(wxCommandEvent &);
    void OnEventButtonBeginUpgrade(wxCommandEvent &);

    void CreateFileFirmware(const wxString &);

    // Вклюить/выключить элементы управления, меняющие состояние при подключении/отключении
    void EnableControlsForConnect(bool);
};
