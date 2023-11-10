// 2022/10/28 22:55:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Screen.h"
#include "Application.h"
#include "Controls/ConsoleSCPI.h"


Frame *Frame::self = nullptr;


enum
{
    FILE_QUIT = wxID_HIGHEST + 1,

    TOOLS_SCPI
};


enum
{
    TIMER_ID = 11111
};


Frame::Frame(const wxString &title)
    :wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    new Screen(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(Screen::self);
    SetSizer(sizer);

    wxMenu *menuFile = new wxMenu();
    wxMenu *menuTools = new wxMenu();

    menuFile->Append(FILE_QUIT, "E&xit\tAlt-X", "Quit this program");
    menuTools->Append(TOOLS_SCPI, "SCPI");

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "Файл");
    menuBar->Append(menuTools, "Инструменты");

    wxFrameBase::SetMenuBar(menuBar);

    SetSizeAndPosition();

    Bind(wxEVT_MENU, &Frame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_MENU, &Frame, OnSCPI, this, TOOLS_SCPI);
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

    Bind(wxEVT_TIMER, &Frame::OnTimer, this, TIMER_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.StartOnce(25);
}


void Frame::OnTimer(wxTimerEvent &)
{
    timer.Stop();

    Application::self->Update();

    timer.Start(25);
}


void Frame::OnQuit(wxCommandEvent &)
{

}


void Frame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::S
}
