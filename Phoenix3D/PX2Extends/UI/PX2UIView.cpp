// PX2UIView.cpp

#include "PX2UIView.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, RenderStep, UIView);

//----------------------------------------------------------------------------
float UIView::msUICameraZ = -2000.0f;
//----------------------------------------------------------------------------
UIView::UIView(int viewID) :
mIsCameraAutoAdjust(true),
mViewID(viewID),
mNotPickedCallback(0),
mMoveAdjugeParam(1.0f),
mMoveAdjugeParamSquare(1.0f)
{
	Camera *camera = new0 Camera(false);
	camera->SetFrame(APoint(0.0f, msUICameraZ, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	SetCamera(camera);

	mCameraNode = new0 CameraNode();
	mCameraNode->SetName("DefaultCameraNode");
	mCameraNode->SetCamera(camera);

#if defined(_WIN32) || defined(WIN32)
	mMoveAdjugeParam = 5.0f;
#else
	mMoveAdjugeParam = 64.0f;
#endif

	mMoveAdjugeParamSquare = mMoveAdjugeParam * mMoveAdjugeParam;

	SetSize(mSize);
}
//----------------------------------------------------------------------------
UIView::~UIView()
{
}
//----------------------------------------------------------------------------
void UIView::Update(double appSeconds, double elapsedSeconds)
{
	if (mCameraNode)
	{
		mCameraNode->Update(appSeconds, elapsedSeconds, false);
	}

	RenderStep::Update(appSeconds, elapsedSeconds);
}
//----------------------------------------------------------------------------
void UIView::SetNode(Node *node)
{
	mNode = node;

	UIFrame *uiFrame = DynamicCast<UIFrame>(node);
	if (uiFrame) uiFrame->SetUIView(this);
}
//----------------------------------------------------------------------------
void UIView::SetSuperTopMoveable(Movable *movable)
{
	mSuperTopMovable = movable;
}
//----------------------------------------------------------------------------
void UIView::PushTopMovable(Movable *movable)
{
	mTopMovables.push_back(movable);
}
//----------------------------------------------------------------------------
void UIView::PopTopMovable()
{
	if (!mTopMovables.empty())
		mTopMovables.pop_back();
}
//----------------------------------------------------------------------------
Movable *UIView::GetTopestMovable()
{
	if (mSuperTopMovable)
		return mSuperTopMovable;

	if (!mTopMovables.empty())
		return mTopMovables.back();

	return 0;
}
//----------------------------------------------------------------------------
void UIView::SetCameraAutoAdjust(bool autoAdjust)
{
	mIsCameraAutoAdjust = autoAdjust;
}
//----------------------------------------------------------------------------
void UIView::SetCameraFrustumSize(const Sizef &size)
{
	mCameraFrustumSize = size;

	float helfWidth = mCameraFrustumSize.Width / 2.0f;
	float helfHeight = mCameraFrustumSize.Height / 2.0f;

	mCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraZ) + 100.0f,
		-helfHeight, helfHeight, -helfWidth, helfWidth);
}
//----------------------------------------------------------------------------
void UIView::SetSize(const Sizef &size)
{
	mSize = size;

	if (mIsCameraAutoAdjust)
	{
		mCameraFrustumSize = mSize;

		SetCameraFrustumSize(mCameraFrustumSize);

		float helfWidth = mCameraFrustumSize.Width / 2.0f;
		float helfHeight = mCameraFrustumSize.Height / 2.0f;
		mCameraNode->LocalTransform.SetTranslate(APoint(helfWidth, msUICameraZ,
			helfHeight));
	}

	if (mNode)
	{
		UIFrame *frame = DynamicCast<UIFrame>(mNode);
		if (frame)
		{
			frame->MarkRelatvieChange();
		}
	}
}
//----------------------------------------------------------------------------
void UIView::DoExecute(Event *event)
{
	if (!InputEventSpace::IsIn(event)) return;

	// get data
	InputEventData data = event->GetData<InputEventData>();
	if (mViewID != data.ViewID) return;

	// accept rect
	bool doPick = true;
	if (!mPickAcceptRect.IsEmpty() &&
		!mPickAcceptRect.IsInsize(Float2(data.MTPos.X(), data.MTPos.Z())))
	{
		doPick = false;
	}

	// events
	if (InputEventSpace::IsEqual(event, InputEventSpace::MouseMoved) ||
		InputEventSpace::IsEqual(event, InputEventSpace::TouchMoved))
	{
		if (doPick)
		{
			_DoPick(data.MTPos.X(), data.MTPos.Z(), UIPT_MOVED, mPickedRenderables);
			mCurPickPos = data.MTPos;
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MousePressed) ||
		InputEventSpace::IsEqual(event, InputEventSpace::TouchPressed))
	{
		mIsPressed = true;

		if (doPick)
		{
			_DoPick(data.MTPos.X(), data.MTPos.Z(), UIPT_PRESSED, mPickedRenderables);
			mPressedPos = data.MTPos;
			mCurPickPos = mPressedPos;
		}
	}
	else if (InputEventSpace::IsEqual(event, InputEventSpace::MouseReleased) ||
		InputEventSpace::IsEqual(event, InputEventSpace::TouchReleased))
	{
		mIsPressed = false;

		if (doPick)
		{
			_DoPick(data.MTPos.X(), data.MTPos.Z(), UIPT_RELEASED, mPickedRenderables);
			mReleasedPos = data.MTPos;
			mCurPickPos = mReleasedPos;
		}
	}
}
//----------------------------------------------------------------------------
void UIView::SetNotPickedCallback(NotPickedCallback callback)
{
	mNotPickedCallback = callback;
}
//----------------------------------------------------------------------------
void UIView::_CollectFrames(Movable *mov, std::set<UIFramePtr> &frames)
{
	if (!mov) return;

	Node *node = DynamicCast<Node>(mov);
	UIFrame *frame = DynamicCast<UIFrame>(mov);
	if (frame)
	{
		frames.insert(frame);
	}

	if (node)
	{
		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			_CollectFrames(node->GetChild(i), frames);
		}
	}
}
//----------------------------------------------------------------------------
void UIView::_DoPick(float x, float z, int pickInfo, 
	std::vector<RenderablePtr> &vec)
{
	// clear
	mPickedFrames.clear();
	vec.clear();

	// get pickMov
	MovablePtr pickMov = 0;
	MovablePtr topestMovable = GetTopestMovable();
	if (topestMovable) pickMov = topestMovable;
	else pickMov = mNode;
	if (!pickMov) return;

	// collect all frames
	std::set<UIFramePtr> frames;
	_CollectFrames(pickMov, frames);

	// pick
	APoint origin;
	AVector direction;
	GetPickRay(x, z, origin, direction);

	Picker pick(true, pickInfo);
	pick.Execute(pickMov, origin, direction, 0.0f, Mathf::MAX_REAL);

	for (int i = 0; i < (int)pick.Records.size(); i++)
	{
		Movable *mov = pick.Records[i].Intersected;
	
		Renderable *renderable = DynamicCast<Renderable>(mov);
		if (renderable) vec.push_back(renderable);
	}

	for (int i = 0; i < (int)vec.size(); i++)
	{
		UIPicBox *picBox = DynamicCast<UIPicBox>(vec[i]);
		if (picBox) picBox->UIPicked(pickInfo);
	}

	if (vec.empty() && mNotPickedCallback)
	{
		mNotPickedCallback(pickInfo);
	}

	// un pick
	std::set<UIFramePtr>::iterator it = frames.begin();
	for (; it != frames.end(); it++)
	{
		UIFrame *frame = *it;
		if (mPickedFrames.find(frame) == mPickedFrames.end())
		{
			frame->OnUINotPicked(pickInfo);
		}
	}
}
//----------------------------------------------------------------------------
