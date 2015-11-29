// PX2UICanvas.cpp

#include "PX2UICanvas.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2InputManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Canvas, UICanvas);

//----------------------------------------------------------------------------
float UICanvas::msUICameraY = -2000.0f;
//----------------------------------------------------------------------------
UICanvas::UICanvas(int viewID) :
mViewID(viewID),
mNotPickedCallback(0),
mMoveAdjugeParam(1.0f),
mMoveAdjugeParamSquare(1.0f),
mParentUIFrame(0),
mIsInittedRefreshCameraSizePos(false),
mScaleCameraFrustumValue(0.0f)
{
	Camera *camera = new0 Camera(false);
	camera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	SetCamera(camera);

	mCameraNode = new0 CameraNode();
	AttachChild(mCameraNode);
	mCameraNode->SetName("DefaultCameraNode");
	mCameraNode->SetCamera(camera);

#if defined(_WIN32) || defined(WIN32)
	mMoveAdjugeParam = 5.0f;
#else
	mMoveAdjugeParam = 64.0f;
#endif

	mMoveAdjugeParamSquare = mMoveAdjugeParam * mMoveAdjugeParam;

	PX2_INPUTMAN.CreateAddListener(viewID);

	SetSizeChangeReAdjustCamera(true);
}
//----------------------------------------------------------------------------
UICanvas::~UICanvas()
{
}
//----------------------------------------------------------------------------
void UICanvas::UpdateWorldData(double applicationTime, double elapsedTime)
{
	SizeNode::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UICanvas::Draw()
{
	if (GetParent())
	{
		APoint worldPos = WorldTransform.GetTranslate();
		APoint leftPos = worldPos + LeftBottomCornerOffset;
		SetViewPort(leftPos.X(), leftPos.Z(), mSize.Width, mSize.Height);
	}

	Canvas::Draw();
}
//----------------------------------------------------------------------------
void UICanvas::SetSuperTopMoveable(Movable *movable)
{
	mSuperTopMovable = movable;
}
//----------------------------------------------------------------------------
void UICanvas::PushTopMovable(Movable *movable)
{
	mTopMovables.push_back(movable);
}
//----------------------------------------------------------------------------
void UICanvas::PopTopMovable()
{
	if (!mTopMovables.empty())
		mTopMovables.pop_back();
}
//----------------------------------------------------------------------------
Movable *UICanvas::GetTopestMovable()
{
	if (mSuperTopMovable)
		return mSuperTopMovable;

	if (!mTopMovables.empty())
		return mTopMovables.back();

	return 0;
}
//----------------------------------------------------------------------------
void UICanvas::TransCameraNode(const AVector &trans)
{
	if (mCameraNode)
	{
		APoint curTrans = mCameraNode->LocalTransform.GetTranslate();
		curTrans += trans;
		mCameraNode->LocalTransform.SetTranslate(curTrans);
	}
}
//----------------------------------------------------------------------------
void UICanvas::ScaleCameraFrustum(float addScaleValue)
{
	mScaleCameraFrustumValue += addScaleValue;

	float dMin = 0.0f;
	float dMax = 0.0f;
	float uMin = 0.0f;
	float uMax = 0.0f;
	float rMin = 0.0f;
	float rMax = 0.0f;
	mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

	float rValue = mSize.Width/2.0f + mScaleCameraFrustumValue;
	float uValue = mSize.Height/2.0f + mScaleCameraFrustumValue * 
		mSize.Height/mSize.Width;
	mCamera->SetFrustum(dMin, dMax, -uValue, uValue, -rValue, rValue);
}
//----------------------------------------------------------------------------
void UICanvas::OnSizeChanged()
{
	if (mIsSizeChangeReAdjustCamera)
	{
		float helfWidth = mSize.Width / 2.0f;
		float helfHeight = mSize.Height / 2.0f;

		mCamera->SetFrustum(0.1f, Mathf::FAbs(msUICameraY) + 100.0f,
			-helfHeight, helfHeight, -helfWidth, helfWidth);

		mCameraNode->LocalTransform.SetTranslate(
			helfWidth + LeftBottomCornerOffset.X(), msUICameraY,
			helfHeight + LeftBottomCornerOffset.Z());
	}

	Canvas::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UICanvas::DoExecute(Event *event)
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
void UICanvas::SetNotPickedCallback(NotPickedCallback callback)
{
	mNotPickedCallback = callback;
}
//----------------------------------------------------------------------------
void UICanvas::_CollectFrames(Movable *mov, std::set<UIFramePtr> &frames)
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
void UICanvas::_DoPick(float x, float z, int pickInfo, 
	std::vector<RenderablePtr> &vec)
{
	// clear
	mPickedFrames.clear();
	vec.clear();

	// get pickMov
	MovablePtr pickMov = 0;
	MovablePtr topestMovable = GetTopestMovable();
	if (topestMovable) pickMov = topestMovable;
	else pickMov = this;
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
UICanvasController *UICanvas::CreateAndAddCanvasController()
{
	if (mCanvasController)
	{
		DetachController(mCanvasController);
		mCanvasController = 0;
	}

	mCanvasController = new0 UICanvasController();
	AttachController(mCanvasController);
	mCanvasController->ComeInEventWorld();
	
	return mCanvasController;
}
//----------------------------------------------------------------------------