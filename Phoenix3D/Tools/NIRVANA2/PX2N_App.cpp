// PX2N_App.cpp

#include "PX2N_App.hpp"
#include "PX2N_MainFrame.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2Edit.hpp"
#include "PX2LuaManager.hpp"
#include "PX2N_PopUpMenuWindow.hpp"
#include "PX2N_GeneralFrame.hpp"
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

	PX2_ENGINELOOP.Play(EngineLoop::PT_NONE);

	wxLog::SetLogLevel(0);

	wxImage::AddHandler(new wxBMPHandler());
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxGIFHandler());
	wxFileSystem::AddHandler(new wxArchiveFSHandler());
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(wxT("DataEditor/wxfbp/*.xrc"));

	int displayWidth, displayHeight;
	wxDisplaySize(&displayWidth, &displayHeight);

	std::string title = "NIRVANA2_";

#if defined(_WIN64) || defined(WIN64)
	title += "64";
#endif

#ifdef _DEBUG
	title += "D";
#endif

	mMainFrame = new N_MainFrame(title, 0, 0, wxMin(800, displayWidth), wxMin(600, displayHeight));

	mMainFrame->Initlize();
	SetTopWindow(mMainFrame);
	mMainFrame->Center();
	//mMainFrame->Maximize();

	PX2_ENGINELOOP.SetPt_Data(mMainFrame->GetHandle());
	PX2_ENGINELOOP.SetPt_Size(Sizef(1024.0f, 768.0f));
	PX2_ENGINELOOP.InitlizeRenderer();

	PX2_EDIT.InitlizeEditor();

	mMainFrame->Show(true);

	return true;
}
//-----------------------------------------------------------------------------
int N_App::OnExit()
{
	PX2_ENGINELOOP.Ternamate();

	return 0;
}
//-----------------------------------------------------------------------------