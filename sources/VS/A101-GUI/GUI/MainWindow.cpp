// 2022/10/28 22:55:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/MainWindow.h"
#include "GUI/Screen.h"
#include "GUI/Application.h"
#include "Controls/ConsoleSCPI.h"


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    static MainFrame *self;

private:

    wxTimer timer;

    void SetSizeAndPosition();

    void OnTimer(wxTimerEvent &);
    void OnClose(wxCloseEvent &);
    void OnQuit(wxCommandEvent &);
    void OnSCPI(wxCommandEvent &);
};


MainFrame *MainFrame::self = nullptr;


enum
{
    FILE_QUIT = wxID_HIGHEST + 1,

    TOOLS_SCPI
};


enum
{
    TIMER_ID = 11111
};


MainFrame::MainFrame(const wxString &title)
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
    menuBar->Append(menuFile, "����");
    menuBar->Append(menuTools, "�����������");

    wxFrameBase::SetMenuBar(menuBar);

    SetSizeAndPosition();

    Bind(wxEVT_MENU, &MainFrame::OnQuit, this, FILE_QUIT);
    Bind(wxEVT_MENU, &MainFrame::OnSCPI, this, TOOLS_SCPI);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    ConsoleSCPI::Self()->Hide();
}


void MainFrame::SetSizeAndPosition()
{
    wxWindowBase::SetClientSize(Screen::WIDTH, Screen::HEIGHT);

    wxTopLevelWindowBase::SetMinSize(wxWindowBase::GetSize());
    wxTopLevelWindowBase::SetMaxSize(wxWindowBase::GetSize());

    const int maxWidth = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    const int maxHeight = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);

    int x = (maxWidth - wxWindowBase::GetSize().x) / 2;
    int y = (maxHeight - wxWindowBase::GetSize().y) / 2;

    wxWindowBase::SetPosition({ x, y });

    Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, TIMER_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.StartOnce(25);
}


void MainFrame::OnTimer(wxTimerEvent &)
{
    timer.Stop();

    Application::self->Update();

    timer.Start(25);
}


void MainFrame::OnQuit(wxCommandEvent &)
{
    Close(true);
}


void MainFrame::OnClose(wxCloseEvent &event)
{
    ConsoleSCPI::Self()->Destroy();

    event.Skip();
}


void MainFrame::OnSCPI(wxCommandEvent &)
{
    ConsoleSCPI::Self()->SwitchVisibility();
}


void MainWindow::Create()
{
    // create and show the main application window
    new MainFrame(_("A101 GUI version"));
    MainFrame::self->Show();
}
