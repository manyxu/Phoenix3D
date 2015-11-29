// PX2N_RenderView.cpp

#include "PX2N_RenderView.hpp"
#include "PX2InputManager.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2EditRenderView.hpp"
#include "PX2EU_Manager.hpp"
using namespace NA;
using namespace PX2;

IMPLEMENT_DYNAMIC_CLASS(NA::RenderView, wxWindow)
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
EVT_RIGHT_DOWN(RenderView::OnRightDown)
EVT_RIGHT_UP(RenderView::OnRightUp)
EVT_MOTION(RenderView::OnMotion)
EVT_KEY_DOWN(RenderView::OnKeyDown)
END_EVENT_TABLE()
//----------------------------------------------------------------------------
RenderView::RenderView()
{
}
//----------------------------------------------------------------------------
RenderView::RenderView(int renderViewID, wxWindow *parent) :
wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER),
mRenderViewID(renderViewID),
mTimerID(-1),
mIsInited(false),
mIsRightDown(false),
mIsRightDownOnMotion(false),
mEditMenu(0)
{
	mTimerID = PX2_EDIT_GETID;
	Connect(mTimerID, wxEVT_TIMER, wxTimerEventHandler(RenderView::OnTimer));

	mTimer.SetOwner(this, mTimerID);
	mTimer.Start(25);

	PX2_EW.ComeIn(this);
}
//----------------------------------------------------------------------------
RenderView::~RenderView()
{
	if (mEditMenu)
	{
		delete mEditMenu;
		mEditMenu = 0;
	}
}
//----------------------------------------------------------------------------
void RenderView::OnTimer(wxTimerEvent& event)
{
	if (mTimerID == event.GetId())
	{
		if (1 == mRenderViewID)
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

	PX2_ENGINELOOP.SetScreenSize(sz);
	EU_Manager *euManager = PX2_EDIT.GetEU_Manager();
	if (euManager)
	{
		UIWindow *uiWindow = euManager->GetUIWindowMain();
		if (uiWindow && uiWindow->GetUICanvas())
		{
			uiWindow->GetUICanvas()->SetSize(sz);
			uiWindow->GetUICanvas()->SetScreenSize(sz);
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
	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->EnterView();

	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void RenderView::OnLeaveWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener)
		listener->LevelView();
}
//----------------------------------------------------------------------------
PX2::APoint wxPointToAPointLeftAxis(wxPoint &point, wxSize &size)
{
	return PX2::APoint((float)point.x, 0.0f, (float)(size.GetHeight() - point.y));
}
//----------------------------------------------------------------------------
void RenderView::OnLeftDown(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener)
		listener->MousePressed(MBID_LEFT, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnLeftUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MouseReleased(MBID_LEFT, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnLeftDoubleClick(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener)
		listener->MouseDoublePressed(MBID_LEFT, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnMiddleDown(wxMouseEvent& e)
{
	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MousePressed(MBID_MIDDLE, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnMiddleUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener)
		listener->MouseReleased(MBID_MIDDLE, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnMouseWheel(wxMouseEvent& e)
{
	float delta = (float)e.GetWheelRotation()*0.1f;

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MouseWheeled(delta);
}
//----------------------------------------------------------------------------
void RenderView::OnRightDown(wxMouseEvent& e)
{
	mIsRightDown = true;
	mIsRightDownOnMotion = false;

	SetFocus();

	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MousePressed(MBID_RIGHT, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnRightUp(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MouseReleased(MBID_RIGHT, pos);
}
//----------------------------------------------------------------------------
void RenderView::OnMotion(wxMouseEvent& e)
{
	wxPoint mousePos = e.GetPosition();
	APoint pos = wxPointToAPointLeftAxis(mousePos, mSize);

	InputEventListener *listener = PX2_INPUTMAN.GetInputListener(mRenderViewID);
	if (listener) 
		listener->MouseMoved(pos);

	if (mIsRightDown)
	{
		mIsRightDownOnMotion = true;
	}
}
//----------------------------------------------------------------------------
void RenderView::OnKeyDown(wxKeyEvent& e)
{
	int keyCode = e.GetKeyCode();
}
//----------------------------------------------------------------------------
void RenderView::OnKeyUp(wxKeyEvent& e)
{
	int keyCode = e.GetKeyCode();
}
//----------------------------------------------------------------------------
void RenderView::DoExecute(PX2::Event *event)
{
}
//----------------------------------------------------------------------------
void RenderView::_CloseEidtRenderView(const std::string &name)
{
	//std::map<std::string, PX2::EditRenderViewPtr>::iterator it
	//	= mEditRenderViews.find(name);
	//if (it != mEditRenderViews.end())
	//{
	//	mEditRenderViews.erase(it);
	//}
}
//----------------------------------------------------------------------------
void RenderView::_NewEditRenderView(const std::string &name)
{
	wxSize size = GetClientSize();
	Sizef sz(size.x, size.y);
	if (sz.Width < 100.0f) sz.Width = 100;
	if (sz.Height < 100.0f) sz.Height = 100;

	EditRenderView *renderView = 0;
}
//----------------------------------------------------------------------------