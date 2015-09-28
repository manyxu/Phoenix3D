// PX2N_MainFrame.cpp

#include "PX2N_MainFrame.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2InputManager.hpp"
#include "PX2EU_Manager.hpp"
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
	mIsSized(false)
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

	mIsInitlized = true;

	return true;
}
//----------------------------------------------------------------------------
void N_MainFrame::OnTimer(wxTimerEvent& e)
{
	PX2_UNUSED(e);

	if (!mIsInitlized) return;
	if (!mIsSized) return;

	PX2_EDIT.IsAltDown = wxGetKeyState(WXK_ALT);
	PX2_EDIT.IsCtrlDown = wxGetKeyState(WXK_CONTROL);
	PX2_EDIT.IsShiftDown = wxGetKeyState(WXK_SHIFT);

	PX2_EDIT.IsKeyDown_W = wxGetKeyState((wxKeyCode)87) || wxGetKeyState((wxKeyCode)119);
	PX2_EDIT.IsKeyDown_S = wxGetKeyState((wxKeyCode)83) || wxGetKeyState((wxKeyCode)115);
	PX2_EDIT.IsKeyDown_A = wxGetKeyState((wxKeyCode)65) || wxGetKeyState((wxKeyCode)97);
	PX2_EDIT.IsKeyDown_D = wxGetKeyState((wxKeyCode)68) || wxGetKeyState((wxKeyCode)100);

	PX2_EDIT.IsLeftMouseDown = wxGetMouseState().LeftIsDown();
	PX2_EDIT.IsRightMouseDown = wxGetMouseState().RightIsDown();
	PX2_EDIT.IsMidMouseDown = wxGetMouseState().MiddleIsDown();

	PX2_ENGINELOOP.Tick();
}
//----------------------------------------------------------------------------
void N_MainFrame::OnSize(wxSizeEvent& e)
{
	if (!mIsInitlized) return;

	mSize  = GetClientSize();
	Sizef sz = Sizef((float)mSize.GetWidth(), (float)mSize.GetHeight());

	PX2_ENGINELOOP.SetScreenSize(sz);
	PX2_EDIT.GetEU_Manager()->GetView_Main()->SetSize(sz);

	mIsSized = true;
}
//----------------------------------------------------------------------------
void N_MainFrame::OnEnterWindow(wxMouseEvent& e)
{
	InputEventListener *listener = PX2_INPUTMAN.GetDefaultListener();
	if (listener) listener->EnterView();
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeaveWindow(wxMouseEvent& e)
{
	InputEventListener *listener = PX2_INPUTMAN.GetDefaultListener();
	if (listener) listener->LevelView();
}
//----------------------------------------------------------------------------
PX2::APoint wxPointToAPointLeftAxis(wxPoint &point, wxSize &size)
{
	return PX2::APoint((float)point.x, 0.0f, (float)(size.GetHeight() - point.y));
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeftDown(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetDefaultListener();
	if (listener) listener->MousePressed(MBID_LEFT, pos);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnLeftUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetDefaultListener();
	if (listener) listener->MouseReleased(MBID_LEFT, pos);
}
//----------------------------------------------------------------------------
void N_MainFrame::OnMotion(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetDefaultListener();
	if (listener) listener->MouseMoved(pos);
}
//----------------------------------------------------------------------------