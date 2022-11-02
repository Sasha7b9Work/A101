// 2022/10/28 22:55:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Screen.h"
#include "Application.h"
#include "Dialogs/DialogNumberComPort.h"
#include "SCPI/SCPI.h"


Frame *Frame::self = nullptr;


enum
{
    TIMER_ID = 11111,

    MENU_FILE_QUIT,

    MENU_SETTINS_NUMPORT
};


Frame::Frame(const wxString &title)
    :wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    new Screen(this);

    CreateMenu();

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(Screen::self);
    SetSizer(sizer);

    SetSizeAndPosition();
}


void Frame::CreateMenu()
{
    wxMenu *menu_file = new wxMenu();
    menu_file->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenu *menu_settings = new wxMenu();
    menu_settings->Append(MENU_SETTINS_NUMPORT, wxT("Номер порта"), wxT("Устновка номера ком-порта"));

    wxMenuBar *bar = new wxMenuBar();
    bar->Append(menu_file, wxT("Файл"));
    bar->Append(menu_settings, wxT("Настройки"));

    SetMenuBar(bar);

    Bind(wxEVT_MENU, &Frame::OnQuitEvent, this, MENU_FILE_QUIT);
    Bind(wxEVT_MENU, &Frame::OnNumberPortEvent, this, MENU_SETTINS_NUMPORT);
}


void Frame::SetSizeAndPosition()
{
    wxWindowBase::SetClientSize(Screen::WIDTH, Screen::HEIGHT);

    wxTopLevelWindowBase::SetMinSize(wxWindowBase::GetSize());
    wxTopLevelWindowBase::SetMaxSize(wxWindowBase::GetSize());

    const int maxWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    const int maxHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

    int x = (maxWidth - wxWindowBase::GetSize().x) / 2;
    int y = (maxHeight - wxWindowBase::GetSize().y) / 2;

    wxWindowBase::SetPosition({ x, y });

    Bind(wxEVT_TIMER, &Frame::OnTimerEvent, this, TIMER_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.StartOnce(25);
}


void Frame::OnTimerEvent(wxTimerEvent &)
{
    timer.Stop();

    Application::self->Update();

    timer.Start(25);
}


void Frame::OnQuitEvent(wxCommandEvent &)
{
    SCPI::DeInit();

    Close(true);
}


void Frame::OnNumberPortEvent(wxCommandEvent &)
{
    DialogNumberComPort dialog;

    dialog.ShowModal();
}
