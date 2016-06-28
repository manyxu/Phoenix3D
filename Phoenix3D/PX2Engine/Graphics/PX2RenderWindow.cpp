// PX2RenderWindow.cpp

#include "PX2RenderWindow.hpp"
#include "PX2Renderer.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsRoot.hpp"

#if defined(_WIN32) || defined (WIN32)
#include <windows.h>
#endif

using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, RenderWindow);

//----------------------------------------------------------------------------
RenderWindow::CursorType RenderWindow::msCursorType = RenderWindow::CT_CURSOR_ARROW;
//----------------------------------------------------------------------------
RenderWindow::RenderWindow() :
mParentRenderWindow(0),
mWindowHandle(0),
mIsMain(false),
mIsFloat(true),
mRenderer(0),
mIsMaxSize(false),
mIsClosed(false),
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
void RenderWindow::Close()
{
	mIsClosed = true;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowClose);
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
void RenderWindow::SetMainCanvas(Canvas *mainCanvas)
{
	mMainCanvas = mainCanvas;
}
//----------------------------------------------------------------------------
Canvas *RenderWindow::GetMainCanvas()
{
	return mMainCanvas;
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
		// clear screen
		mRenderer->SetViewport(Rectf(0.0f, 0.0f, mScreenSize.Width, mScreenSize.Height));
		mRenderer->SetClearColor(Float4(230.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f));

		mRenderer->ClearBuffers();

		if (mMainCanvas && mMainCanvas->IsShow())
		{
			mMainCanvas->Draw(mRenderer);
		}

		mRenderer->ClearDepthBuffer();

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