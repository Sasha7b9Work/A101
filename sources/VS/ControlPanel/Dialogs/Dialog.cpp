#include "defines.h"
#include "Dialogs/Dialog.h"


#define BUTTON_SIZE wxSize(70, 30)


Dialog::Dialog(const wxString &title) : wxDialog(nullptr, wxID_ANY, title)
{
    Connect(wxEVT_MOVE, wxMoveEventHandler(Dialog::OnMove));

    wxButton *btnOk = new wxButton(this, wxID_ANY, wxT("Принять"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(btnOk->GetId(), wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonApply));
    wxButton *btnCancel = new wxButton(this, wxID_ANY, wxT("Отменить"), wxDefaultPosition, BUTTON_SIZE); //-V2511
    Connect(btnCancel->GetId(), wxEVT_BUTTON, wxCommandEventHandler(Dialog::OnButtonCancel));

    Bind(wxEVT_KEY_DOWN, &Dialog::OnKeyDown, this);
    Bind(wxEVT_KEY_UP, &Dialog::OnKeyDown, this);

    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Dialog::OnKeyDown));
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(Dialog::OnKeyDown));

    wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL); //-V2511
    panelBox = new wxBoxSizer(wxVERTICAL); //-V2511
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL); //-V2511

    vBox->Add(panelBox);
    hBox->Add(btnOk, 1, wxALIGN_CENTER);
    hBox->AddSpacer(20);
    hBox->Add(btnCancel, 1, wxALIGN_CENTER);
    vBox->AddSpacer(10);
    vBox->Add(hBox, 0, wxALIGN_CENTER);

    SetSizer(vBox);
}


Dialog::~Dialog()
{
}


void Dialog::SetBoxSizer(wxBoxSizer *sizer, wxSize size)
{
    panelBox->Add(sizer);

    size.y += 50;

    SetClientSize(size);

    Centre();

    SetFocus();
}


void Dialog::OnMove(wxMoveEvent &)
{
}


void Dialog::OnButtonApply(wxCommandEvent &)
{
    ApplyParameters();
    Destroy();
}


void Dialog::OnButtonCancel(wxCommandEvent &)
{
    CancelParameters();
    Destroy();
}


void Dialog::OnKeyDown(wxKeyEvent &)
{
}
