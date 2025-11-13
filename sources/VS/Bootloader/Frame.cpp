// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Controls/Sizer.h"
#include "Utils/GlobalFunctions.h"
#include "Communicator/ComPort.h"
#include "Upgrader.h"
#include "File.h"


Frame *Frame::self = nullptr;


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);
    Bind(wxEVT_SIZE, &Frame::OnSize, this);

    wxWindow *window = new wxWindow(this, wxID_ANY);

    wxSize size_label{ 90, 15 };

    sizer_file = CreateSizerFile(window);

    sizer_device = CreateSizerDevice(window, size_label);

    sizer_version = CreateSizerVersion(window, size_label);

    sizer_date = CreateSizerDate(window, size_label);

    CreatePanel(window);

    SetClientSize(450, 190);

    window->SetSize({450, 190});

    Refresh();

    GF::SendCommandEvent(btnUpdatePorts, wxEVT_BUTTON, btnUpdatePorts->GetId());
}


void Frame::OnSize(wxSizeEvent &event)
{
    Layout();

    event.Skip();
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    event.Skip();
}


void Frame::CreatePanel(wxWindow *window)
{
    wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
    {
        txtPort = new wxStaticText(window, wxID_ANY, _("COM-порт"));

        cbComPorts = new wxComboBox(window, wxID_ANY, wxEmptyString, wxDefaultPosition, { 80, 20 });

        btnUpdatePorts = new wxButton(window, wxID_ANY, _("Обновить"));
        btnUpdatePorts->Bind(wxEVT_BUTTON, &Frame::OnEventButtonUpdatePorts, this);

        btnConnect = new wxButton(window, wxID_ANY, _("Подключиться"));
        btnConnect->Bind(wxEVT_BUTTON, &Frame::OnEventButtonConnect, this);

        top->AddSpacer(10);
        top->Add(txtPort);
        top->AddSpacer(10);
        top->Add(cbComPorts);
        top->AddSpacer(10);
        top->Add(btnUpdatePorts);
        top->AddSpacer(50);
        top->Add(btnConnect);
        top->AddSpacer(10);
    }

    wxBoxSizer *window_sizer = new wxBoxSizer(wxVERTICAL);

    window_sizer->AddSpacer(10);
    window_sizer->Add(top);
    window_sizer->AddSpacer(20);
    window_sizer->Add(sizer_device);
    window_sizer->AddSpacer(30);
    window_sizer->Add(sizer_version);
    window_sizer->AddSpacer(10);
    window_sizer->Add(sizer_date);
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
        btnSelectFile = new wxButton(window, wxID_ANY, _("Выбор файла"), wxDefaultPosition, { 200, 23 });
        btnSelectFile->Bind(wxEVT_BUTTON, &Frame::OnEventButtonSelectFile, this);

        btnUpgradeFirmware = new wxButton(window, wxID_ANY, _("Firmware update"));
        btnUpgradeFirmware->Enable(false);
        btnUpgradeFirmware->Bind(wxEVT_BUTTON, &Frame::OnEventButtonBeginUpgrade, this);

        sizer->AddSpacer(25);
        sizer->Add(btnSelectFile);
        sizer->AddSpacer(25);
        sizer->Add(btnUpgradeFirmware);
    }

    return sizer;
}


Sizer *Frame::CreateSizerVersion(wxWindow *window, const wxSize &size_label)
{
    Sizer *sizer = new Sizer(wxHORIZONTAL);

    {
        wxStaticText *txtVersionLabel = new wxStaticText(window, wxID_ANY, _("Версия"), wxDefaultPosition, size_label);
        txtVersionValue = new wxStaticText(window, wxID_ANY, "-", wxDefaultPosition, size_label);

        sizer->AddSpacer(50);
        sizer->Add(txtVersionLabel);
        sizer->AddSpacer(10);
        sizer->Add(txtVersionValue);
        sizer->AddSpacer(10);
    }

    return sizer;
}


void Frame::SetDeviceInfo(const wxString &info)
{
    txtDeviceInfo->SetLabel(info);
}


Sizer *Frame::CreateSizerDevice(wxWindow *window, const wxSize &size_label)
{
    Sizer *sizer = new Sizer(wxHORIZONTAL);

    {
        wxStaticText *txtVersionLabel = new wxStaticText(window, wxID_ANY, _("A101"), wxDefaultPosition, size_label);
        txtDeviceInfo = new wxStaticText(window, wxID_ANY, "-", wxDefaultPosition, size_label);

        sizer->AddSpacer(50);
        sizer->Add(txtVersionLabel);
        sizer->AddSpacer(10);
        sizer->Add(txtDeviceInfo);
        sizer->AddSpacer(10);
    }

    return sizer;
}


wxBoxSizer *Frame::CreateSizerDate(wxWindow *window, const wxSize &size_label)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    {
        wxStaticText *txtDateLabel = new wxStaticText(window, wxID_ANY, _("Дата сборки"), wxDefaultPosition, size_label);
        txtDateValue = new wxStaticText(window, wxID_ANY, "-", wxDefaultPosition, size_label);

        sizer->AddSpacer(50);
        sizer->Add(txtDateLabel);
        sizer->AddSpacer(10);
        sizer->Add(txtDateValue);
        sizer->AddSpacer(10);
    }

    return sizer;
}


void Frame::OnEventButtonSelectFile(wxCommandEvent &)
{
    wxFileDialog dialog(self, "Открыть файл с прошивкой", wxEmptyString, wxEmptyString, "*.bin", wxFD_OPEN);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString file_name = dialog.GetPath();

        btnSelectFile->SetLabel(file_name);

        btnSelectFile->SetToolTip(file_name);

        CreateFileFirmware(file_name);
    }

    btnUpgradeFirmware->Enable(File::IsValid());
}


void Frame::CreateFileFirmware(const wxString &file_name)
{
    File::Create(file_name);

    if (File::IsValid())
    {
        txtVersionValue->SetLabel(wxString::Format("%u", File::GetVersion()));

        txtDateValue->SetLabel(File::GetDateBuild());
    }
    else
    {
        txtVersionValue->SetLabel("");

        txtDateValue->SetLabel("Не является файлом прошивки");
    }
}


void Frame::OnEventButtonUpdatePorts(wxCommandEvent &)
{
    std::vector<bool> &ports = ComPort::GetComports();

    cbComPorts->Clear();

    for (size_t i = 0; i < ports.size(); i++)
    {
        if (ports[i])
        {
            cbComPorts->Append(wxString::Format("COM%u", i + 1U));
        }
    }

    cbComPorts->SetSelection(0);

    btnConnect->Enable(cbComPorts->GetCount() != 0);
}


void Frame::OnEventButtonConnect(wxCommandEvent &)
{
    if (btnConnect->GetLabel() == "Подключиться")
    {
        TryConnect();
    }
    else
    {
        Disconnect();
    }
}


void Frame::Disconnect()
{
    ComPort::Close();

    EnableControlsForConnect(true);
}


void Frame::TryConnect()
{
    wxString name_port = cbComPorts->GetValue();

    wxString num_port;

    pchar digit = name_port.c_str().AsChar() + name_port.Length() - 1;

    while (*digit >= '0' && *digit <= '9')
    {
        num_port.Prepend(*digit);
        digit--;
    }

    long number = 0;

    num_port.ToLong(&number);

    if (ComPort::TryConnect((int)number - 1))
    {
        Upgrader::Reset();

        EnableControlsForConnect(false);
    }
}


void Frame::EnableControlsForConnect(bool enable)
{
    txtPort->Enable(enable);
    cbComPorts->Enable(enable);
    btnUpdatePorts->Enable(enable);

    btnConnect->SetLabel(enable ? "Подключиться" : "Отключиться");
}


void Frame::OnEventButtonBeginUpgrade(wxCommandEvent &)
{
    Upgrader::CommandBeginUpgrade();
}
