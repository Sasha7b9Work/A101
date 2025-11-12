// 2022/04/29 13:56:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Frame.h"
#include "Communicator/ComPort.h"


wxIMPLEMENT_APP(Application);


bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    // create and show the main application window
    Frame *frame = new Frame(_("A101 Устройство программирования"));

    Init();

    Bind(wxEVT_TIMER, &Application::OnTimer, this, timer.GetId());

    timer.SetOwner(this, timer.GetId());

    timer.Start();

    frame->Show();

    return true;
}


void Application::OnTimer(wxTimerEvent &)
{
    ComPort::Update();
}


int Application::OnExit()
{
    return wxApp::OnExit();
}


void Application::Init()
{

}
