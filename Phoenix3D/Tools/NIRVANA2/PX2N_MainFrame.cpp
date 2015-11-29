// PX2N_MainFrame.cpp

#include "PX2N_MainFrame.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2InputManager.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
using namespace NA;
using namespace PX2;

const int sID_ENGINELOOPTIMER = PX2_EDIT_GETID;

BEGIN_EVENT_TABLE(N_MainFrame, wxFrame)
EVT_TIMER(sID_ENGINELOOPTIMER, N_MainFrame::OnTimer)
EVT_SIZE(N_MainFrame::OnSize)
EVT_ENTER_WINDOW(N_MainFrame::OnEnterWindow)
EVT_LEAVE_WINDOW(N_MainFrame::OnLeaveWindow)
EVT_LEFT_DOWN(N_MainFrame::OnLeftDown)
EVT_LEFT_UP(N_MainFrame::OnLeftUp)
EVT_MOTION(N_MainFrame::OnMotion)
END_EVENT_TABLE();

//----------------------------------------------------------------------------
N_MainFrame::N_MainFrame(const std::string &title, int xPos, int yPos,
	int width, int height):
	wxFrame((wxFrame*)0, -1, title, wxPoint(xPos, yPos), wxSize(width, height)),
	mIsInitlized(false),
	mIsSized(false),
	mMainRenderView(0)
{
}
//----------------------------------------------------------------------------
N_MainFrame::~N_MainFrame()
{
}
//----------------------------------------------------------------------------
bool N_MainFrame::Initlize()
{
	mTimer.SetOwner(this, sID_ENGINELOOPTIMER);
	mTimer.Start(25);

	_CreateMenu();

	mMainRenderView = new RenderView(0, this);
	PX2_ENGINELOOP.SetPt_Data(mMainRenderView->GetHandle());
	PX2_ENGINELOOP.SetPt_Size(Sizef(1024.0f, 768.0f));
	PX2_ENGINELOOP.InitlizeRenderer();

	mIsInitlized = true;

	return true;
}
//----------------------------------------------------------------------------
void N_MainFrame::DoExecute(Event *event)
{
	if (EditEventSpace::IsEqual(event, EditEventSpace::AddMainMenu))
	{
		EED_AddMenu_Main data = event->GetData<EED_AddMenu_Main>();
		
		if (data.IsMain_Main)
		{
			AddMainMenuItem(data.Title);
		}
	}
}
//----------------------------------------------------------------------------
void N_MainFrame::OnTimer(wxTimerEvent& e)
{
	PX2_UNUSED(e);

	if (!mIsInitlized) return;
	if (!mIsSized) return;
}
//----------------------------------------------------------------------------
void N_MainFrame::OnSize(wxSizeEvent& e)
{
	wxSize size = GetClientSize();
	if (mMainRenderView)
		mMainRenderView->SetSize(0, 0, size.x, size.y);

	mIsSized = true;
}
//----------------------------------------------------------------------------
void N_MainFrame::OnEnterWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeaveWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeftDown(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeftUp(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnMotion(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
RenderView *N_MainFrame::GetMainRenderView()
{
	return mMainRenderView;
}
//----------------------------------------------------------------------------