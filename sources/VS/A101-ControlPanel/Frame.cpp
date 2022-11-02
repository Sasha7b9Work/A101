// 2022/10/28 22:55:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Screen.h"
#include "Application.h"


Frame *Frame::self = nullptr;


enum
{
    TIMER_ID = 11111,

    MENU_FILE_QUIT
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
    wxMenu *file_menu = new wxMenu();

    file_menu->Append(MENU_FILE_QUIT, wxT("Выход\tAlt+X"), wxT("Закрыть редактор"));

    wxMenuBar *menu_bar = new wxMenuBar();

    menu_bar->Append(file_menu, wxT("Файл"));

    SetMenuBar(menu_bar);

    Bind(wxEVT_MENU, &Frame::OnQuitEvent, this, MENU_FILE_QUIT);
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
    Close(true);
}
