// PX2N_App.cpp

#include "PX2L_App.hpp"
#include "PX2L_Frame.hpp"
#include "PX2EditEventData.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2Edit.hpp"
#include "PX2EditEventType.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Memory.hpp"
#include "PX2UIFrame.hpp"
using namespace PX2;
using namespace Launcher;

//-----------------------------------------------------------------------------
IMPLEMENT_APP(Launcher::L_App)
//-----------------------------------------------------------------------------
L_App::L_App()
{
}
//-----------------------------------------------------------------------------
L_App::~L_App()
{
}
//-----------------------------------------------------------------------------
bool L_App::OnInit()
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

	L_Frame *frame = CreateMainFrame("Main");

	frame->Show(true);

	return true;
}
//-----------------------------------------------------------------------------
int L_App::OnExit()
{
	PX2_EW.GoOut(this);
	PX2_ENGINELOOP.Terminate();

	return 0;
}
//-----------------------------------------------------------------------------
L_Frame *L_App::CreateMainFrame(const std::string &name)
{
	int displayWidth, displayHeight;
	wxDisplaySize(&displayWidth, &displayHeight);

	std::string title = "Launcher";

#if defined(_WIN64) || defined(WIN64)
	title += "64";
#endif

#ifdef _DEBUG
	title += "D";
#endif

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetScreenSize(Sizef(1024.0f, 768.0f));
	rw->SetTitle(title);

	// load dll
	std::string debugTag = "";
#if defined (_DEBUG) 
	debugTag = "D";
#endif
	std::string lpDllPath = "LPlugin" + debugTag + ".dll";
	PX2_PLUGINMAN.Load(lpDllPath);

	// create wxFrame
	L_Frame *mainFrame = new L_Frame(rw, 0, wxDEFAULT_FRAME_STYLE);
	mainFrame->Show(false);
	SetTopWindow(mainFrame);
	mainFrame->Center();

	const Sizef &size = PX2_GR.GetMainWindow()->GetScreenSize();
	PX2_ENGINELOOP.SetPt_Data(mainFrame->GerRenderView()->GetHandle());
	PX2_ENGINELOOP.SetPt_Size(size);
	PX2_ENGINELOOP.InitlizeRenderer();

	mainFrame->Show(true);

	return mainFrame;
}
//-----------------------------------------------------------------------------