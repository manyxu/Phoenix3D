// PX2RenderWindow.cpp

#include "PX2RenderWindow.hpp"
#include "PX2Renderer.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"

#if defined(_WIN32) || defined (WIN32)
#include <windows.h>
#endif

using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, RenderWindow);

//----------------------------------------------------------------------------
int RenderWindow::sNextWindowID = 0;
RenderWindow::CursorType RenderWindow::msCursorType = RenderWindow::CT_CURSOR_ARROW;
//----------------------------------------------------------------------------
RenderWindow::RenderWindow() :
mParentRenderWindow(0),
mWindowHandle(0),
mIsMain(false),
mIsFloat(true),
mIsCanvasesNeedReSort(true),
mRenderer(0),
mIsMaxSize(false),
mIsShow(true),
mIsDraging(false),
mIsCaptureMouse(false),
mIsLeftDown(false)
{
	ComeInEventWorld();
}
//----------------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void RenderWindow::SetWindowHandle(void *handle)
{
	mWindowHandle = handle;
}
//----------------------------------------------------------------------------
void *RenderWindow::GetWindowHandle()
{
	return mWindowHandle;
}
//----------------------------------------------------------------------------
void RenderWindow::SetMain(bool main)
{
	mIsMain = main;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsMain() const
{
	return mIsMain;
}
//----------------------------------------------------------------------------
void RenderWindow::SetTitle(const std::string &title)
{
	mTitle = title;
}
//----------------------------------------------------------------------------
const std::string &RenderWindow::GetTitle() const
{
	return mTitle;
}
//----------------------------------------------------------------------------
void RenderWindow::SetFloat(bool fl)
{
	mIsFloat = fl;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsFloat() const
{
	return mIsFloat;
}
//----------------------------------------------------------------------------
void RenderWindow::SetPosition(const APoint &pos)
{
	mPosition = pos;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowSetPosition);
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::SetScreenSize(const Sizef &size)
{
	mScreenSize = size;

	if (mMainCanvas)
	{
		mMainCanvas->SetSize(size);
	}

	if (mRenderer)
	{
		mRenderer->ResizeWindow((int)mScreenSize.Width,
			(int)mScreenSize.Height);
	}
}
//----------------------------------------------------------------------------
void RenderWindow::MaxSize(bool maxSize)
{
	mIsMaxSize = maxSize;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowMaxSize);
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::Show(bool show)
{
	mIsShow = show;

	Event *ent = 0;
	if (show)
	{
		ent = GraphicsES::CreateEventX(GraphicsES::WindowShow);
	}
	else
	{
		ent = GraphicsES::CreateEventX(GraphicsES::WindowHide);
	}
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::SetDraging(bool draging)
{
	mIsDraging = draging;
}
//----------------------------------------------------------------------------
void RenderWindow::ShowCursor(bool showCursor)
{
#if defined(_WIN32) || defined (WIN32)

	bool bIsCursorVisible = IsShowCursor();
	if (showCursor && !bIsCursorVisible)
	{
		// Restore the old mouse position when we show the cursor.
		if (mPreCaptureMousePos.X() >= 0.0f && mPreCaptureMousePos.Z() >= 0.0f)
		{
			::SetCursorPos((int)mPreCaptureMousePos.X(), 
				(int)mPreCaptureMousePos.Z());
		}
		while (::ShowCursor(TRUE) < 0);

		mPreCaptureMousePos.X() = -1;
		mPreCaptureMousePos.Z() = -1;
	}
	else if (!showCursor && bIsCursorVisible)
	{
		while (::ShowCursor(FALSE) >= 0);

		// Remember the current mouse position when we hide the cursor.
		mPreCaptureMousePos.X() = -1;
		mPreCaptureMousePos.Z() = -1;

		POINT pos;
		::GetCursorPos(&pos);
		mPreCaptureMousePos.X() = (float)pos.x;
		mPreCaptureMousePos.Z() = (float)pos.y;
	}

#endif

}
//----------------------------------------------------------------------------
bool RenderWindow::IsShowCursor() const
{
#if defined(_WIN32) || defined (WIN32)

	CURSORINFO CursorInfo;
	CursorInfo.cbSize = sizeof(CURSORINFO);
	bool bIsVisible = (::GetCursorInfo(&CursorInfo) != 0);
	bIsVisible = bIsVisible && bool(CursorInfo.flags & 
		CURSOR_SHOWING) && bool(CursorInfo.hCursor != NULL);
	return bIsVisible;

#endif
}
//----------------------------------------------------------------------------
void RenderWindow::CaptureMouse(bool capture)
{
#if defined(_WIN32) || defined (WIN32)
	HWND captureWindow = ::GetCapture();
	bool bIsMouseCaptured = (captureWindow == (HWND)mWindowHandle);

	mIsCaptureMouse = capture;

	if (capture && !bIsMouseCaptured)
	{
		::SetCapture((HWND)mWindowHandle);
	}
	else if (!capture && bIsMouseCaptured)
	{
		::ReleaseCapture();
	}
#endif
}
//----------------------------------------------------------------------------
void RenderWindow::SetParentRenderWindow(RenderWindow *rw)
{
	mParentRenderWindow = rw;
}
//----------------------------------------------------------------------------
int RenderWindow::GetNextWindowID()
{
	return sNextWindowID++;
}
//----------------------------------------------------------------------------
void RenderWindow::SetMainCanvas(Canvas *mainCanvas)
{
	mMainCanvas = mainCanvas;
	mMainCanvas->SetRenderWindow(this);
}
//----------------------------------------------------------------------------
Canvas *RenderWindow::GetMainCanvas()
{
	return mMainCanvas;
}
//----------------------------------------------------------------------------
bool CanvasLessThan(const Canvas *canvas0, const Canvas *canvas1)
{
	if (canvas0->IsMain() && !canvas1->IsMain())
		return true;

	if (!canvas0->IsMain() && canvas1->IsMain())
		return false;

	if (canvas0->WorldTransform.GetTranslate().Y() == 
		canvas1->WorldTransform.GetTranslate().Y())
	{
		return canvas0 < canvas1;
	}
	else
	{
		return canvas0->WorldTransform.GetTranslate().Y() <
			canvas1->WorldTransform.GetTranslate().Y();
	}
}
//----------------------------------------------------------------------------
bool RenderWindow::AddCanvas(Canvas *canvas)
{
	if (IsHasCanvas(canvas))
		return false;

	mCanvases.push_back(canvas);

	mIsCanvasesNeedReSort = true;

	return true;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsHasCanvas(Canvas *canvas) const
{
	for (int i = 0; i < (int)mCanvases.size(); i++)
	{
		if (canvas == mCanvases[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool RenderWindow::RemoveCanvas(Canvas *canvas)
{
	auto it = mCanvases.begin();
	for (; it != mCanvases.end(); it++)
	{
		if (canvas == *it)
		{
			mCanvases.erase(it);
			mIsCanvasesNeedReSort = true;
			return true;
		}
	}
	
	return false;
}
//----------------------------------------------------------------------------
void RenderWindow::SetRenderer(Renderer *renderer)
{
	mRenderer = renderer;
}
//----------------------------------------------------------------------------
void RenderWindow::Update(double appSeconds, double elapsedSeconds)
{
	if (mMainCanvas)
	{
		mMainCanvas->Update(appSeconds, elapsedSeconds);
	}

	if (mIsCanvasesNeedReSort)
	{
		std::sort(mCanvases.begin(), mCanvases.end(), CanvasLessThan);
		mIsCanvasesNeedReSort = false;
	}
}
//----------------------------------------------------------------------------
void RenderWindow::ComputeVisibleSet()
{
	if (mMainCanvas)
	{
		mMainCanvas->ComputeVisibleSet();
	}
}
//----------------------------------------------------------------------------
void RenderWindow::Draw()
{
	if (mRenderer && mRenderer->PreDraw())
	{
		if (mMainCanvas && mMainCanvas->IsShow())
		{
			mMainCanvas->Draw(mRenderer);
		}

		mRenderer->PostDraw();
		mRenderer->DisplayColorBuffer();
	}
}
//----------------------------------------------------------------------------
void RenderWindow::SetCursorType(CursorType ct)
{
	msCursorType = ct;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowSetCursorType);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
RenderWindow::CursorType RenderWindow::GetCursorType()
{
	return msCursorType;
}
//----------------------------------------------------------------------------
void _TravelSetRenderWindow(Movable *mov, Any *data, bool &goOn)
{
	Canvas *canvas = DynamicCast<Canvas>(mov);
	if (canvas)
	{
		RenderWindow *rw = PX2_ANY_AS(*data, RenderWindow*);
		canvas->SetRenderWindow(rw);
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnEvent(Event *event)
{
	if (InputEventSpace::IsIn(event))
	{
		// get data
		InputEventData data = event->GetData<InputEventData>();
		int viewID = data.ViewID;
		if (viewID != GetID())
			return;

		APoint worldPos = data.MTPos;

		for (int i = 0; i < (int)mCanvases.size(); i++)
		{
			Canvas *canvas = mCanvases[i];
			if (!canvas->IsShow() || !canvas->IsEnable() || !canvas->IsActivated())
				continue;

			const Rectf &worldRect = canvas->GetWorldRect();
			if (worldRect.IsInsize(worldPos.X(), worldPos.Z()))
			{
				if (InputEventSpace::IsEqual(event, InputEventSpace::MousePressed) ||
					InputEventSpace::IsEqual(event, InputEventSpace::TouchPressed))
				{
					if (MBID_LEFT == data.MButtonID)
					{
						canvas->OnLeftDown(worldPos);
					}
					else if (MBID_RIGHT == data.MButtonID)
					{
						canvas->OnRightDown(worldPos);
					}
					else if (MBID_MIDDLE == data.MButtonID)
					{
						canvas->OnMiddleDown(worldPos);
					}
				}
				else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseReleased) ||
					InputEventSpace::IsEqual(event, InputEventSpace::TouchReleased))
				{
					if (MBID_LEFT == data.MButtonID)
					{
						canvas->OnLeftUp(worldPos);
					}
					else if (MBID_RIGHT == data.MButtonID)
					{
						canvas->OnRightUp(worldPos);
					}
					else if (MBID_MIDDLE == data.MButtonID)
					{
						canvas->OnMiddleUp(worldPos);
					}
				}
				else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseDoublePressed))
				{
					if (MBID_LEFT == data.MButtonID)
					{
						canvas->OnLeftDClick(worldPos);
					}
				}
				else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseMoved) ||
					InputEventSpace::IsEqual(event, InputEventSpace::TouchMoved))
				{
					canvas->OnMotion(worldPos);
				}
				else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseWheeled))
				{
					canvas->OnMouseWheel(worldPos, data.MWheel);
				}

				if (canvas->IsPickedSomething())
					return;

				if (!IsMain())
					return;
			}
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::AddObject))
	{
		AddObjectData data = event->GetData<AddObjectData>();
		Object *parent = data.ParentObj;
		Object *obj = data.Obj;
		Canvas *canvas = DynamicCast<Canvas>(obj);
		if (canvas)
		{
			Canvas *topestCanvas = canvas->GetFirstParentDerivedFromType<Canvas>();
			if (topestCanvas)
			{
				RenderWindow *rw = topestCanvas->GetRenderWindow();
				if (rw)
				{
					Any rwData = rw;
					Node::TravelExecute(topestCanvas, _TravelSetRenderWindow,
						&rwData);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------