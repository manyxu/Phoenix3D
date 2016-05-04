// PX2RenderWindow.cpp

#include "PX2RenderWindow.hpp"
#include "PX2Renderer.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2GraphicsEventType.hpp"

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

	std::map<FString, CanvasPtr>::iterator it = mCanvasMap.begin();
	for (; it != mCanvasMap.end(); it++)
	{
		Canvas *canvas = it->second;
		canvas->SetScreenSize(size);

		if (canvas->IsMain())
		{
			canvas->SetSize(size);
		}
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
bool RenderWindow::AddCanvas(const std::string &name, Canvas *canvas)
{
	canvas->SetName(name);

	if (IsHasCanvas(name))
		return false;

	if ("Scene" == name) mCanvasScene = canvas;
	if ("UI" == name) mCanvasUI = canvas;

	mCanvasMap[name.c_str()] = canvas;
	mCanvasVec.push_back(canvas);

	canvas->SetRenderWindow(this);

	SortCanvass();

	_CollectCanvas();

	return true;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsHasCanvas(const std::string &name) const
{
	return mCanvasMap.find(name.c_str()) != mCanvasMap.end();
}
//----------------------------------------------------------------------------
bool RenderWindow::RemoveCanvas(const std::string &name)
{
	Canvas *canvas = GetCanvas(name);
	return RemoveCanvas(canvas);
}
//----------------------------------------------------------------------------
bool RenderWindow::RemoveCanvas(Canvas *canvas)
{
	std::vector<Canvas*>::iterator itVec = mCanvasVec.begin();
	for (; itVec != mCanvasVec.end();)
	{
		if (*itVec == canvas)
		{
			itVec = mCanvasVec.erase(itVec);
		}
		else
		{
			itVec++;
		}
	}

	std::map<FString, CanvasPtr>::iterator it = mCanvasMap.begin();
	for (; it != mCanvasMap.end(); it++)
	{
		if (it->second == canvas)
		{
			canvas->SetRenderWindow(0);

			if (mCanvasScene == canvas)
				mCanvasScene = 0;

			if (mCanvasUI == canvas)
				mCanvasUI = 0;

			mCanvasMap.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void RenderWindow::RemoveAllCanvas()
{
	mCanvasVec.clear();
	mCanvasMap.clear();

	mCanvasScene = 0;
	mCanvasUI = 0;
}
//----------------------------------------------------------------------------
Canvas *RenderWindow::GetCanvas(const std::string &name)
{
	std::map<FString, CanvasPtr>::iterator it = mCanvasMap.find(name.c_str());

	if (it != mCanvasMap.end())
		return it->second;

	return 0;
}
//----------------------------------------------------------------------------
void RenderWindow::SortCanvass()
{
	std::sort(mCanvasVec.begin(), mCanvasVec.end(),
		Canvas::LessThan);
}
//----------------------------------------------------------------------------
void RenderWindow::SetRenderer(Renderer *renderer)
{
	mRenderer = renderer;
}
//----------------------------------------------------------------------------
void RenderWindow::Update(double appSeconds, double elapsedSeconds)
{
	for (int i = 0; i < (int)mCanvasVec.size(); i++)
	{
		// 只调用 topCanvas 的Update
		if (!mCanvasVec[i]->GetParent())
		{
			mCanvasVec[i]->Update(appSeconds, elapsedSeconds);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::ComputeVisibleSetAndEnv()
{
	for (int i = 0; i < (int)mCanvasVec.size(); i++)
	{
		mCanvasVec[i]->ClearVisibleSet();
		mCanvasVec[i]->ComputeVisibleSetAndEnv();
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

		for (int i = 0; i < (int)mCanvasVec.size(); i++)
		{
			if (mCanvasVec[i]->IsShow())
				mCanvasVec[i]->Draw(mRenderer);
		}

		mRenderer->PostDraw();
		mRenderer->DisplayColorBuffer();
	}
}
//----------------------------------------------------------------------------
void RenderWindow::DoExecute(Event *event)
{
	if (!IsActivated())
		return;

	int id = GetID();

	if (InputEventSpace::IsIn(event))
	{
		// get data
		InputEventData data = event->GetData<InputEventData>();
		if (data.ViewID != id)
			return;

		APoint worldPos = data.MTPos;

		if (InputEventSpace::IsEqual(event, InputEventSpace::MousePressed) ||
			InputEventSpace::IsEqual(event, InputEventSpace::TouchPressed))
		{
			if (MBID_LEFT == data.MButtonID)
			{
				OnLeftDown(worldPos);
			}
			else if (MBID_RIGHT == data.MButtonID)
			{
				OnRightDown(worldPos);
			}
			else if (MBID_MIDDLE == data.MButtonID)
			{
				OnMiddleDown(worldPos);
			}
		}
		else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseReleased) ||
			InputEventSpace::IsEqual(event, InputEventSpace::TouchReleased))
		{
			if (MBID_LEFT == data.MButtonID)
			{
				OnLeftUp(worldPos);
			}
			else if (MBID_RIGHT == data.MButtonID)
			{
				OnRightUp(worldPos);
			}
			else if (MBID_MIDDLE == data.MButtonID)
			{
				OnMiddleUp(worldPos);
			}
		}
		else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseMoved) ||
			InputEventSpace::IsEqual(event, InputEventSpace::TouchMoved))
		{
			OnMotion(worldPos);
		}
		else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseWheeled))
		{
			OnMouseWheel(data.MWheel, worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void CanvasTravelExecuteFun (Movable *mov, Any *data, bool &isGoOn)
{	
	Canvas *canvas = DynamicCast<Canvas>(mov);
	if (canvas)
	{
		RenderWindow *rw = PX2_ANY_AS(*data, RenderWindow*);
		rw->mCanvasCollect.push_back(canvas);
	}

	PX2_UNUSED(isGoOn);
}
//----------------------------------------------------------------------------
void RenderWindow::_CollectCanvas()
{
	mCanvasCollect.clear();

	for (int i = 0; i < (int)mCanvasVec.size(); i++)
	{
		Canvas *canvas = mCanvasVec[i];
		if (canvas)
		{
			Any dt = this;
			Node::TravelExecute(canvas, CanvasTravelExecuteFun, &dt);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnLeftDown(const APoint &worldPos)
{
	_CollectCanvas();

	mIsLeftDown = true;

	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			Rectf rect = canvas->GetWorldRect();
			bool isInRange = rect.IsInsize(worldPos.X(), worldPos.Z());

			if (isInRange || !canvas->IsPickOnlyInSizeRange())
				canvas->OnLeftDown(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnLeftUp(const APoint &worldPos)
{
	mIsLeftDown = false;

	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnLeftUp(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnLeftDClick(const APoint &worldPos)
{
	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];		

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z())
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnLeftDClick(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnMiddleDown(const APoint &worldPos)
{
	_CollectCanvas();

	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z())
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnMiddleDown(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnMiddleUp(const APoint &worldPos)
{
	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnMiddleUp(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnMouseWheel(float delta, const APoint &worldPos)
{
	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnMouseWheel(worldPos, delta);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnRightDown(const APoint &worldPos)
{
	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnRightDown(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnRightUp(const APoint &worldPos)
{
	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable() && canvas->IsActivated())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnRightUp(worldPos);
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::OnMotion(const APoint &worldPos)
{
	if (!IsMain())
	{
		if (mIsLeftDown)
		{
			APoint pos = worldPos;
			//SetPosition(pos);

			//PX2_LOG_INFO("Pos: %f, %f", pos.X(), pos.Z());
		}
	}

	for (int i = 0; i < (int)mCanvasCollect.size(); i++)
	{
		Canvas *canvas = mCanvasCollect[i];

		if (canvas->IsEnable())
		{
			if (canvas->GetWorldRect().IsInsize(worldPos.X(), worldPos.Z()) 
				|| !canvas->IsPickOnlyInSizeRange())
				canvas->OnMotion(worldPos);
		}
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