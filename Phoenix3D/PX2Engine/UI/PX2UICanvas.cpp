// PX2UICanvas.cpp

#include "PX2UICanvas.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2InputManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Canvas, UICanvas);
PX2_IMPLEMENT_STREAM(UICanvas);
PX2_IMPLEMENT_FACTORY(UICanvas);
PX2_IMPLEMENT_DEFAULT_NAMES(Canvas, UICanvas);

//----------------------------------------------------------------------------
float UICanvas::msUICameraY = -2000.0f;
//----------------------------------------------------------------------------
UICanvas::UICanvas() :
mMoveAdjugeParam(1.0f),
mMoveAdjugeParamSquare(1.0f),
mIsInittedRefreshCameraSizePos(false),
mScaleCameraFrustumValue(0.0f),
mIsNeedAdjustChildrenMask(true)
{
	mCamera = new0 Camera(false);
	mCamera->SetFrame(APoint(0.0f, msUICameraY, 0.0f), AVector::UNIT_Y,
		AVector::UNIT_Z, AVector::UNIT_X);
	AddCamera(mCamera);

	mCameraNode = new0 CameraNode();
	AttachChild(mCameraNode);
	mCameraNode->SetName("DefaultCameraNode");
	mCameraNode->SetCamera(mCamera);

#if defined(_WIN32) || defined(WIN32)
	mMoveAdjugeParam = 5.0f;
#else
	mMoveAdjugeParam = 64.0f;
#endif

	mMoveAdjugeParamSquare = mMoveAdjugeParam * mMoveAdjugeParam;

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
	EnableAnchorLayout(true);

	mCanvasRenderBind->SetRenderLayer(Renderable::RL_UI, 1);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
UICanvas::~UICanvas()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void UICanvas::OnChildAttached(Movable *child)
{
	Canvas::OnChildAttached(child);

	Any canvasData = this;
	Node::TravelExecute(child, UIFrame::TravelExecuteSetCanvas, &canvasData);
}
//----------------------------------------------------------------------------
void UICanvas::OnChildDetach(Movable *child)
{
	Canvas::OnChildDetach(child);

	Any canvasData = (UICanvas*)0;
	Node::TravelExecute(child, UIFrame::TravelExecuteSetCanvas, &canvasData);
}
//----------------------------------------------------------------------------
void _TravelSetMask(Movable *mov, Any *data, bool &goOn)
{
	UIFrame *frame = DynamicCast<UIFrame>(mov);
	Renderable *renderable = DynamicCast<Renderable>(mov);
	if (frame)
	{
		UICanvas *canvas = DynamicCast<UICanvas>(frame->GetParent());
		UIFrame *parentFrame = DynamicCast<UIFrame>(frame->GetParent());

		if (canvas)
		{
			if (!frame->GetMask())
			{
				frame->SetMaskVal(0);
			}
			else
			{
				frame->SetMaskVal(1);
			}
		}
		else if (parentFrame)
		{
			if (!frame->GetMask())
			{
				frame->SetMaskVal(parentFrame->GetMaskVal());
			}
			else
			{
				frame->SetMaskVal(parentFrame->GetMaskVal() + 1);
			}
		}
	}
	else if (renderable)
	{
		UIFrame *parentFrame = DynamicCast<UIFrame>(renderable->GetParent());
		if (parentFrame)
		{
			int parentMask = parentFrame->GetMaskVal();
			Canvas *canvas = DynamicCast<Canvas>(parentFrame->GetParent());

			StencilProperty *sp = renderable->GetMaterialInstance()->GetMaterial()
				->GetStencilProperty(0, 0);

			if ("MaskPicBox" == renderable->GetName())
			{
				if (canvas)
				{
					sp->Enabled = true;
					sp->Reference = parentMask;
					sp->Compare = StencilProperty::CM_ALWAYS;
					sp->OnZPass = StencilProperty::OT_REPLACE;
				}
				else
				{
					sp->Enabled = true;
					sp->Reference = parentMask - 1;
					sp->Compare = StencilProperty::CM_EQUAL;
					sp->OnZPass = StencilProperty::OT_INCREMENT;
				}
			}
			else
			{
				sp->Enabled = true;
				sp->Reference = parentMask;
				sp->Compare = StencilProperty::CM_EQUAL;
			}
		}
	}
}
//----------------------------------------------------------------------------
void UICanvas::SetNeedAdjustChildrenMask(bool isNeed)
{
	mIsNeedAdjustChildrenMask = isNeed;
}
//----------------------------------------------------------------------------
bool UICanvas::IsNeedAdjustChildrenMask() const
{
	return mIsNeedAdjustChildrenMask;
}
//----------------------------------------------------------------------------
void UICanvas::_AdjustChildrenMask()
{
	Node::TravelExecute(this, _TravelSetMask);

	mIsNeedAdjustChildrenMask = false;
}
//----------------------------------------------------------------------------
void UICanvas::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNeedAdjustChildrenMask)
	{
		_AdjustChildrenMask();
	}

	Canvas::UpdateWorldData(applicationTime, elapsedTime);
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
APoint UICanvas::ScreenPosToCameraPos(const APoint &screenPos)
{
	Transform inverseTrans = WorldTransform.InverseTransform();
	APoint localPos = inverseTrans * screenPos;

	float xDelta = (localPos.X() / mSize.Width) * mCamera->GetRMax() * 2.0f;
	float zDelta = (localPos.Z() / mSize.Height) * mCamera->GetUMax() * 2.0f;

	APoint worldRealPos = mCamera->GetPosition() + AVector(xDelta, 0.0f, 
		zDelta);

	return worldRealPos;
}
//----------------------------------------------------------------------------
void UICanvas::OnSizeChanged()
{
	_UpdateCamera();

	Canvas::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UICanvas::UpdateLeftBottomCornerOffset(Movable *parent)
{
	Canvas::UpdateLeftBottomCornerOffset(parent);

	_UpdateCamera();
}
//----------------------------------------------------------------------------
void UICanvas::_UpdateCamera()
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
	else
	{
		float dMin = 0.0f;
		float dMax = 0.0f;
		float uMin = 0.0f;
		float uMax = 0.0f;
		float rMin = 0.0f;
		float rMax = 0.0f;
		mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

		float rValue = mSize.Width / 2.0f + mScaleCameraFrustumValue;
		float uValue = mSize.Height / 2.0f + mScaleCameraFrustumValue *
			mSize.Height / mSize.Width;
		mCamera->SetFrustum(dMin, dMax, -uValue, uValue, -rValue, rValue);
	}
}
//----------------------------------------------------------------------------
void UICanvas::_AddPickWidget(UIFrame *widget)
{
	mPickWidgets.push_back(widget);
}
//----------------------------------------------------------------------------
void UICanvas::_RemovePickWidget(UIFrame *widget)
{
	std::list<UIFrame*>::iterator it = std::find(mPickWidgets.begin(), 
		mPickWidgets.end(), widget);
	if (it != mPickWidgets.end())
	{
		mPickWidgets.erase(it);
	}
}
//----------------------------------------------------------------------------
void UICanvas::_AddInRangePickWidget(UIFrame *widget)
{
	mInRangeWidgets.push_back(widget);
}
//----------------------------------------------------------------------------
bool _UIFrameFrontThan(const UIFrame *frame0,
	const UIFrame *frame1)
{
	float posY0 = frame0->WorldTransform.GetTranslate().Y();
	float posY1 = frame1->WorldTransform.GetTranslate().Y();

	if (posY0 == posY1)
	{
		return frame0 < frame1;
	}
	else
	{
		return posY0 < posY1;
	}
}
//----------------------------------------------------------------------------
void UICanvas::_SortInRangePickWidget()
{
	mInRangeWidgets.sort(_UIFrameFrontThan);
}
//----------------------------------------------------------------------------
void UICanvas::_DoPick(const UIInputData &inputData)
{
	mInRangeWidgets.clear();

	for (int i = 0; i < GetNumChildren(); i++)
	{
		UIFrame *uiFrame = DynamicCast<UIFrame>(GetChild(i));
		if (uiFrame)
		{
			uiFrame->PreUIPick(inputData, this);
		}
	}

	UIFrame *inRangeWidget = 0;
	if ((int)mInRangeWidgets.size() > 0)
	{
		_SortInRangePickWidget();
		inRangeWidget = *(mInRangeWidgets.begin());
	}

	if (inRangeWidget)
	{
		inRangeWidget->OnUIPicked(inputData);
	}

	std::list<UIFrame*>::iterator it = mPickWidgets.begin();
	for (; it != mPickWidgets.end(); it++)
	{
		if (inRangeWidget != *it)
		{
			(*it)->OnUINotPicked(inputData);
		}
	}
}
//----------------------------------------------------------------------------
UICanvasController *UICanvas::CreateAndAddCanvasController()
{
	DestoryCanvasController();

	mCanvasController = new0 UICanvasController();
	AttachController(mCanvasController);
	mCanvasController->ComeInEventWorld();

	SetSizeChangeReAdjustCamera(false);
	
	return mCanvasController;
}
//----------------------------------------------------------------------------
void UICanvas::DestoryCanvasController()
{
	if (mCanvasController)
	{
		mCanvasController->GoOutEventWorld();
		DetachController(mCanvasController);
		mCanvasController = 0;

		SetSizeChangeReAdjustCamera(true);
	}
}
//----------------------------------------------------------------------------
void UICanvas::OnLeftDown(const APoint &worldPos)
{
	Canvas::OnLeftDown(worldPos);
	
	CalPixelToWorld();

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_LEFT;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_PRESSED;

	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnLeftUp(const APoint &worldPos)
{
	Canvas::OnLeftUp(worldPos);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_LEFT;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnLeftDClick(const APoint &worldPos)
{
	PX2_UNUSED(worldPos);
}
//----------------------------------------------------------------------------
void UICanvas::OnMiddleDown(const APoint &worldPos)
{
	Canvas::OnMiddleDown(worldPos);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_MIDDLE;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnMiddleUp(const APoint &worldPos)
{
	Canvas::OnMiddleUp(worldPos);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_MIDDLE;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnMouseWheel(const APoint &worldPos, float delta)
{
	Canvas::OnMouseWheel(worldPos, delta);

	PX2_UNUSED(delta);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_MIDDLE;
	inputData.WorldPos = worldPos;
	inputData.Wheel = delta;
	inputData.PickType = UIPT_WHELLED;

	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnRightDown(const APoint &worldPos)
{
	Canvas::OnRightDown(worldPos);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_RIGHT;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_PRESSED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnRightUp(const APoint &worldPos)
{
	Canvas::OnRightUp(worldPos);

	UIInputData inputData;
	inputData.TheMouseTag = UIInputData::MT_RIGHT;
	inputData.WorldPos = worldPos;
	inputData.PickType = UIPT_RELEASED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------
void UICanvas::OnMotion(const APoint &worldPos)
{
	Canvas::OnMotion(worldPos);

	UIInputData inputData;
	inputData.WorldPos = worldPos;
	inputData.MoveDelta = mMoveDelta;
	inputData.PickType = UIPT_MOVED;
	_DoPick(inputData);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UICanvas::UICanvas(LoadConstructor value) :
Canvas(value)
{
}
//----------------------------------------------------------------------------
void UICanvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Canvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UICanvas, source);
}
//----------------------------------------------------------------------------
void UICanvas::Link(InStream& source)
{
	Canvas::Link(source);
}
//----------------------------------------------------------------------------
void UICanvas::PostLink()
{
	Canvas::PostLink();
}
//----------------------------------------------------------------------------
bool UICanvas::Register(OutStream& target) const
{
	return Canvas::Register(target);
}
//----------------------------------------------------------------------------
void UICanvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Canvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UICanvas, target);
}
//----------------------------------------------------------------------------
int UICanvas::GetStreamingSize(Stream &stream) const
{
	int size = Canvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------