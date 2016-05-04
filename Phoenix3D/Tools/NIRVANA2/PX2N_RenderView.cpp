// PX2N_RenderView.cpp

#include "PX2N_RenderView.hpp"
#include "PX2InputManager.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2N_Frame.hpp"
#include "PX2Time.hpp"
using namespace NA;
using namespace PX2;

BEGIN_EVENT_TABLE(RenderView, wxWindow)
EVT_SIZE(RenderView::OnSize)
EVT_PAINT(RenderView::OnPaint)
EVT_ERASE_BACKGROUND(RenderView::OnEraseBackground)
EVT_ENTER_WINDOW(RenderView::OnEnterWindow)
EVT_LEAVE_WINDOW(RenderView::OnLeaveWindow)
EVT_LEFT_DOWN(RenderView::OnLeftDown)
EVT_LEFT_UP(RenderView::OnLeftUp)
EVT_LEFT_DCLICK(RenderView::OnLeftDoubleClick)
EVT_MIDDLE_DOWN(RenderView::OnMiddleDown)
EVT_MIDDLE_UP(RenderView::OnMiddleUp)
EVT_MOUSEWHEEL(RenderView::OnMouseWheel)
EVT_MOTION(RenderView::OnMotion)
EVT_RIGHT_DOWN(RenderView::OnRightDown)
EVT_RIGHT_UP(RenderView::OnRightUp)
EVT_KEY_DOWN(RenderView::OnKeyDown)
EVT_KEY_UP(RenderView::OnKeyUp)
EVT_CHAR(RenderView::OnChar)
END_EVENT_TABLE()
//----------------------------------------------------------------------------
RenderView::RenderView()
{
}
//----------------------------------------------------------------------------
RenderView::RenderView(RenderWindow *rw, wxWindow *parent) :
	wxWindow(parent, wxID_ANY, wxDefaultPosition,
	wxSize(rw->GetScreenSize().Width, rw->GetScreenSize().Height),
wxNO_BORDER | wxTRANSPARENT_WINDOW),
mTimerID(-1),
mIsInited(false),
mIsRightDown(false),
mIsRightDownOnMotion(false),
mRenderWindow(rw)
{
	std::string name = rw->GetName();
	SetName(name);

	mTimerID = PX2_EDIT_GETID;
	Connect(mTimerID, wxEVT_TIMER, wxTimerEventHandler(RenderView::OnTimer));

	mTimer.SetOwner(this, mTimerID);
	mTimer.Start(25);

	PX2_EW.ComeIn(this);

	SetFocus();

	if ("Main" == name)
	{

	}
	else
	{
		PX2_INPUTMAN.CreateAddListener(mTimerID);
		rw->SetID(mTimerID);
	}

	const Sizef &size = rw->GetScreenSize();
	mSize.SetWidth((int)size.Width);
	mSize.SetHeight((int)size.Height);
	SetClientSize(mSize);
}
//----------------------------------------------------------------------------
RenderView::~RenderView()
{
}
//----------------------------------------------------------------------------
void RenderView::OnTimer(wxTimerEvent& event)
{
	if (mTimerID == event.GetId())
	{
		std::string name = GetName();
		if ("Main" == name)
		{
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
	}
}
//----------------------------------------------------------------------------
void RenderView::OnSize(wxSizeEvent& e)
{
	PX2_UNUSED(e);

	mSize = GetClientSize();
	Sizef sz = Sizef((float)mSize.GetWidth(), (float)mSize.GetHeight());
	std::string name = GetName();

	if ("Main" == name)
	{
		PX2_ENGINELOOP.SetScreenSize(sz);
	}
	else
	{
		if (mRenderWindow)
		{
			mRenderWindow->SetScreenSize(sz);
		}
	}
}
//----------------------------------------------------------------------------
void RenderView::OnPaint(wxPaintEvent& e)
{
	wxPaintDC dc(this);
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void RenderView::OnEraseBackground(wxEraseEvent& e)
{
	if (!mIsInited) return;
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void RenderView::OnEnterWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->EnterView();
	}
}
//----------------------------------------------------------------------------
void RenderView::OnLeaveWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->LevelView();
	}
}
//----------------------------------------------------------------------------
PX2::APoint wxPointToAPointRightAxis(wxPoint &point, wxSize &size)
{
	return PX2::APoint((float)point.x, 0.0f, (float)(size.GetHeight() - point.y));
}
//----------------------------------------------------------------------------
void RenderView::OnLeftDown(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MousePressed(MBID_LEFT, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MousePressed(MBID_LEFT, pos);
	}

	SetFocus();
}
//----------------------------------------------------------------------------
void RenderView::OnLeftUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseReleased(MBID_LEFT, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseReleased(MBID_LEFT, pos);
	}
}
//----------------------------------------------------------------------------
void RenderView::OnLeftDoubleClick(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseDoublePressed(MBID_LEFT, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseDoublePressed(MBID_LEFT, pos);
	}
}
//----------------------------------------------------------------------------
void RenderView::OnMiddleDown(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MousePressed(MBID_MIDDLE, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MousePressed(MBID_MIDDLE, pos);
	}

	SetFocus();
}
//----------------------------------------------------------------------------
void RenderView::OnMiddleUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseReleased(MBID_MIDDLE, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseReleased(MBID_MIDDLE, pos);
	}
}
//----------------------------------------------------------------------------
void RenderView::OnMouseWheel(wxMouseEvent& e)
{
	float delta = (float)e.GetWheelRotation()*0.1f;
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseWheeled(delta, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseWheeled(delta, pos);
	}
}
//----------------------------------------------------------------------------
void RenderView::OnRightDown(wxMouseEvent& e)
{
	mIsRightDown = true;
	mIsRightDownOnMotion = false;

	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MousePressed(MBID_RIGHT, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MousePressed(MBID_RIGHT, pos);
	}

	SetFocus();
}
//----------------------------------------------------------------------------
void RenderView::OnRightUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseReleased(MBID_RIGHT, pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseReleased(MBID_RIGHT, pos);
	}
}
//----------------------------------------------------------------------------
void RenderView::OnMotion(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointRightAxis(mousePos, mSize);

	std::string name = GetName();
	if ("Main" == name)
	{
		PX2_INPUTMAN.GetDefaultListener()->MouseMoved(pos);
	}
	else
	{
		PX2_INPUTMAN.GetInputListener(mTimerID)->MouseMoved(pos);
	}

	if (mIsRightDown)
	{
		mIsRightDownOnMotion = true;
	}
}
//----------------------------------------------------------------------------
KeyCode ConverKeyCode(int wParam)
{
	KeyCode code = KC_UNASSIGNED;
	if (0x41 == wParam) code = KC_A;
	else if (0x42 == wParam) code = KC_B;
	else if (0x43 == wParam) code = KC_C;
	else if (0x44 == wParam) code = KC_D;
	else if (0x45 == wParam) code = KC_E;
	else if (0x46 == wParam) code = KC_F;
	else if (0x47 == wParam) code = KC_G;
	else if (0x48 == wParam) code = KC_H;
	else if (0x49 == wParam) code = KC_I;
	else if (0x4A == wParam) code = KC_J;
	else if (0x4B == wParam) code = KC_K;
	else if (0x4C == wParam) code = KC_L;
	else if (0x4D == wParam) code = KC_M;
	else if (0x4E == wParam) code = KC_N;
	else if (0x4F == wParam) code = KC_O;
	else if (0x50 == wParam) code = KC_P;
	else if (0x51 == wParam) code = KC_Q;
	else if (0x52 == wParam) code = KC_R;
	else if (0x53 == wParam) code = KC_S;
	else if (0x54 == wParam) code = KC_T;
	else if (0x55 == wParam) code = KC_U;
	else if (0x56 == wParam) code = KC_V;
	else if (0x57 == wParam) code = KC_W;
	else if (0x58 == wParam) code = KC_X;
	else if (0x59 == wParam) code = KC_Y;
	else if (0x5A == wParam) code = KC_Z;
	return code;
}
//----------------------------------------------------------------------------
void RenderView::OnKeyDown(wxKeyEvent& e)
{
	int keyCode = e.GetKeyCode();

	KeyCode kc = ConverKeyCode(keyCode);
	PX2_INPUTMAN.GetDefaultListener()->KeyPressed(kc);

	e.Skip();
}
//----------------------------------------------------------------------------
void RenderView::OnKeyUp(wxKeyEvent& e)
{
	int keyCode = e.GetKeyCode();

	KeyCode kc = ConverKeyCode(keyCode);
	PX2_INPUTMAN.GetDefaultListener()->KeyReleased(kc);

	e.Skip();
}
//----------------------------------------------------------------------------
void RenderView::OnChar(wxKeyEvent& e)
{
	int keyCode = e.GetKeyCode();

	if (keyCode < 32)
	{
		if (WXK_BACK == keyCode)
		{
			IMEDispatcher::GetSingleton().DispathDeleteBackward();
		}
		else if (WXK_RETURN == keyCode)
		{
			IMEDispatcher::GetSingleton().DispathInsertText("\n", 1);
		}
		else if (WXK_TAB == keyCode)
		{
			// tab input
		}
		else if (WXK_ESCAPE == keyCode)
		{
			// ESC input
		}
	}
	else if (keyCode < 128)
	{
		IMEDispatcher::GetSingleton().DispathInsertText((const char*)&keyCode, 1);
	}
}
//----------------------------------------------------------------------------
void RenderView::DoExecute(PX2::Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------