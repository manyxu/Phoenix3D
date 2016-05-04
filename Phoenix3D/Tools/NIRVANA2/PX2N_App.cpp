// PX2N_App.cpp

#include "PX2N_App.hpp"
#include "PX2N_Frame.hpp"
#include "PX2N_GeneralFrame.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditEventData.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2Edit.hpp"
#include "PX2EditEventType.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2EditEventType.hpp"
using namespace PX2;
using namespace NA;

//-----------------------------------------------------------------------------
IMPLEMENT_APP(NA::N_App)
//-----------------------------------------------------------------------------
N_App::N_App()
{

}
//-----------------------------------------------------------------------------
N_App::~N_App()
{
}
//-----------------------------------------------------------------------------
bool N_App::OnInit()
{
	PX2_ENGINELOOP.Initlize();
	PX2_EW.ComeIn(this);

	PX2_ENGINELOOP.Play(EngineLoop::PT_NONE);

	wxLog::SetLogLevel(0);

	wxImage::AddHandler(new wxBMPHandler());
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxGIFHandler());
	wxFileSystem::AddHandler(new wxArchiveFSHandler());
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(wxT("DataNIRVANA2/wxfbp/*.xrc"));


	N_Frame *frame = CreateMainFrame();
	frame->Maximize();

	const Sizef &size = PX2_GR.GetMainWindow()->GetScreenSize();
	PX2_ENGINELOOP.SetPt_Data(frame->GerRenderView()->GetHandle());
	PX2_ENGINELOOP.SetPt_Size(size);
	PX2_ENGINELOOP.InitlizeRenderer();
	PX2_EDIT.InitlizeEditor();

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetWindowHandle(frame->GerRenderView()->GetHandle());

	frame->Show(true);

	return true;
}
//-----------------------------------------------------------------------------
int N_App::OnExit()
{
	PX2_EW.GoOut(this);
	PX2_ENGINELOOP.Terminate();

	return 0;
}
//-----------------------------------------------------------------------------
void N_App::DoExecute(Event *event)
{
	if (EditEventSpace::IsEqual(event, EditEventSpace::N_Simu))
	{
		int type = event->GetData<int>();
		SimuApp(type);
	}
}
//-----------------------------------------------------------------------------
N_Frame *N_App::CreateMainFrame()
{
	int displayWidth, displayHeight;
	wxDisplaySize(&displayWidth, &displayHeight);

	std::string title = "NIRVANA2";

#if defined(_WIN64) || defined(WIN64)
	title += "_64";
#endif

#ifdef _DEBUG
	title += "_D";
#endif

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetTitle(title);
	N_Frame *mainFrame = new N_Frame(rw, 0, wxDEFAULT_FRAME_STYLE);
	mainFrame->Show(false);
	SetTopWindow(mainFrame);
	mainFrame->Center();

	return mainFrame;
}
//-----------------------------------------------------------------------------
void N_App::SimuApp(int type)
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	const std::string &projName = proj->GetName();

	EngineLoop::PlayType pt = PX2_ENGINELOOP.GetPlayType();
	if (1 == type)
	{	
		if (pt != EngineLoop::PT_NONE)
		{
			PX2_ENGINELOOP.Play(EngineLoop::PT_NONE);
		}
		else
		{
			PX2_ENGINELOOP.Play(EngineLoop::PT_SIMULATE);
		}
	}
	else if (2 == type)
	{
		if (pt != EngineLoop::PT_NONE)
		{
			PX2_ENGINELOOP.Play(EngineLoop::PT_NONE);
		}
		else
		{
			PX2_ENGINELOOP.Play(EngineLoop::PT_PLAY);
		}
	}
	else if (3 == type)
	{
		std::string appName;

#if defined(_WIN32) || defined(WIN32)

#ifdef _DEBUG
#if defined(_WIN64) || defined(WIN64)
		appName = "AppPlayer64D.exe" + std::string(" ") + projName;
#else
		appName = "AppPlayerD.exe" + std::string(" ") + projName;
#endif
#else
#if defined(_WIN64) || defined(WIN64)
		appName = "AppPlayer64.exe" + std::string(" ") + projName;
#else
		appName = "AppPlayer.exe" + std::string(" ") + projName;
#endif
#endif
		WinExec(appName.c_str(), SW_SHOW);
#endif
	}
}
//-----------------------------------------------------------------------------