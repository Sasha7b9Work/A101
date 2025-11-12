// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Controls/Sizer.h"


Frame *Frame::self = nullptr;


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_SIZE, &Frame::OnSize, this);

    wxWindow *window = new wxWindow(this, wxID_ANY);

    sizer_file = CreateSizerFile(window);

    CreatePanel(window);

    SetClientSize(470, 215);

    sizer_version->Enable(false);
    sizer_file->Enable(false);
}


void Frame::OnSize(wxSizeEvent &event)
{
    Layout();

    event.Skip();
}


void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    event.Skip();
}


void Frame::CreatePanel(wxWindow *window)
{
    wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
    {
        wxStaticText *txtPort = new wxStaticText(window, wxID_ANY, _("COM-порт"));
        wxComboBox *cbComPort = new wxComboBox(window, wxID_ANY, wxEmptyString, wxDefaultPosition, { 80, 20 });
        wxButton *btnUpdate = new wxButton(window, wxID_ANY, _("Обновить"));
        wxButton *btnConnect = new wxButton(window, wxID_ANY, _("Подключиться"));

        top->AddSpacer(10);
        top->Add(txtPort);
        top->AddSpacer(10);
        top->Add(cbComPort);
        top->AddSpacer(10);
        top->Add(btnUpdate);
        top->AddSpacer(50);
        top->Add(btnConnect);
        top->AddSpacer(10);
    }

    wxSize size_label{ 90, 15 };

    sizer_version = new Sizer(wxHORIZONTAL);
    {
        wxStaticText *txtVersionLabel = new wxStaticText(window, wxID_ANY, _("Версия"), wxDefaultPosition, size_label);
        wxStaticText *txtVersionValue = new wxStaticText(window, wxID_ANY, "-", wxDefaultPosition, size_label);

        sizer_version->AddSpacer(50);
        sizer_version->Add(txtVersionLabel);
        sizer_version->AddSpacer(10);
        sizer_version->Add(txtVersionValue);
        sizer_version->AddSpacer(10);
    }

    wxBoxSizer *box_date = new wxBoxSizer(wxHORIZONTAL);
    {
        wxStaticText *txtDateLabel = new wxStaticText(window, wxID_ANY, _("Дата сборки"), wxDefaultPosition, size_label);
        wxStaticText *txtDateValue = new wxStaticText(window, wxID_ANY, "-", wxDefaultPosition, size_label);

        box_date->AddSpacer(50);
        box_date->Add(txtDateLabel);
        box_date->AddSpacer(10);
        box_date->Add(txtDateValue);
        box_date->AddSpacer(10);
    }

    wxBoxSizer *window_sizer = new wxBoxSizer(wxVERTICAL);

    window_sizer->AddSpacer(10);
    window_sizer->Add(top);
    window_sizer->AddSpacer(20);
    window_sizer->Add(sizer_version);
    window_sizer->AddSpacer(10);
    window_sizer->Add(box_date);
    window_sizer->AddSpacer(20);
    window_sizer->Add(sizer_file);
    window_sizer->AddSpacer(10);

    window->SetSizer(window_sizer);

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    sizer->Add(window);

    SetSizer(sizer);
}


Sizer *Frame::CreateSizerFile(wxWindow *window)
{
    Sizer *sizer = new Sizer(wxHORIZONTAL);

    {
        wxButton *btnSelectFile = new wxButton(window, wxID_ANY, _("Выбор файла"), wxDefaultPosition, { 250, 23 });
        wxButton *btnUpdate = new wxButton(window, wxID_ANY, _("Обновить"));

        btnUpdate->Enable(false);

        sizer->AddSpacer(25);
        sizer->Add(btnSelectFile);
        sizer->AddSpacer(25);
        sizer->Add(btnUpdate);
    }

    return sizer;
}
