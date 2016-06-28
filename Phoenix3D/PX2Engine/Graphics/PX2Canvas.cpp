// PX2UIView.cpp

#include "PX2Canvas.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, Canvas);
PX2_IMPLEMENT_STREAM(Canvas);
PX2_IMPLEMENT_FACTORY(Canvas);
PX2_IMPLEMENT_DEFAULT_NAMES(SizeNode, Canvas);

//----------------------------------------------------------------------------
Canvas::Canvas() :
mRenderNode(0),
mRenderWindow(0),
mIsMain(false),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mIsPickOnlyInSizeRange(true),
mIsInRange(false),
mIsPressed(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false)
{
	mOverrideWireProperty = new0 WireProperty();
	mOverrideWireProperty->Enabled = true;

	mPixelToWorld.first = 0.0f;
	mPixelToWorld.second = 0.0f;

	mCanvasRenderBind = new0 CanvasRenderBind();
	AttachChild(mCanvasRenderBind);

	VertexColor4MaterialPtr mtl = new0 VertexColor4Material();
	AlphaProperty *alphaProperty = mtl->GetAlphaProperty(0, 0);
	alphaProperty->BlendEnabled = true;
	MaterialInstance *mtlInst = mtl->CreateUniqueInstance();
	mtlInst->GetPass(0)->GetAlphaProperty()->BlendEnabled = true;

	mCanvasRenderBind->SetMaterialInstance(mtlInst);

	SetColor(Float3::WHITE);
	SetAlpha(1.0f);
	SetBrightness(1.0f);
	SetColorSelfCtrled(true);
	SetAlphaSelfCtrled(true);
	SetBrightnessSelfCtrled(true);

	VertexFormat *vfPoly = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);
	mDebugPolyVB = new0 VertexBuffer(1000, vfPoly->GetStride(), Buffer::BU_DYNAMIC);
	mDebugPoly = new0 Polysegment(vfPoly, mDebugPolyVB, false);
	mDebugPoly->SetNumSegments(0);
	MaterialInstance *mi = VertexColor4Material::CreateUniqueInstance();
	mDebugPoly->SetMaterialInstance(mi);

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Canvas::~Canvas()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void Canvas::SetMain(bool main)
{
	mIsMain = main;
}
//----------------------------------------------------------------------------
void Canvas::SetRenderNode(SizeNode *movable)
{
	mRenderNode = movable;
}
//----------------------------------------------------------------------------
void Canvas::SetRenderWindow(RenderWindow *rw)
{
	mRenderWindow = rw;
}
//----------------------------------------------------------------------------
void Canvas::OnSizeChanged()
{
	SizeNode::OnSizeChanged();

	if (GetParent())
	{
		APoint worldPos = WorldTransform.GetTranslate();
		APoint leftPos = worldPos + LeftBottomCornerOffset;
		SetViewPort(leftPos.X(), leftPos.Z(), mSize.Width, mSize.Height);
	}

	CalPixelToWorld();
}
//----------------------------------------------------------------------------
void Canvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	ClearDebugLine();

	if (mIsLayoutChanged)
	{
		if (mIsAnchorLayoutEnable)
		{
			UpdateLayout(GetParent());
			UpdateLeftBottomCornerOffset(GetParent());
		}
		else
		{
			UpdateLeftBottomCornerOffset(GetParent());
		}

		if (mRenderNode)
		{
			mRenderNode->_MarkRelatvieChange();

			mRenderNode->UpdateLayout(GetParent());
			mRenderNode->UpdateLeftBottomCornerOffset(GetParent());
		}

		mIsLayoutChanged = false;
	}

	SizeNode::UpdateWorldData(applicationTime, elapsedTime);

	if (mRenderNode)
	{
		mRenderNode->Update(applicationTime, elapsedTime);
	}
}
//----------------------------------------------------------------------------
void Canvas::UpdateWorldBound()
{
	// 如果bound为0，我们需要重新计算一个bound

	if (!WorldBoundIsCurrent)
	{
		bool foundFirstBound = false;

		WorldBound.SetCenter(APoint::ORIGIN);
		WorldBound.SetRadius(0.0f);

		for (int i = 0; i < (int)mChild.size(); i++)
		{
			Movable *child = mChild[i];

			if (child)
			{
				if (foundFirstBound)
				{
					// 将所有孩子的节点包围体融合到一起，作为当前节点的包围体
					WorldBound.GrowToContain(child->WorldBound);
				}
				else
				{
					// 首先将第一个孩子的包围盒作为节点的包围盒
					foundFirstBound = true;
					WorldBound = child->WorldBound;
				}
			}
		}

		if (0.0f == WorldBound.GetRadius())
		{
			float radius = Mathf::Sqrt(mSize.Width*mSize.Width +
				mSize.Height + mSize.Height) / 2.0f;
			WorldBound.SetCenter(WorldTransform.GetTranslate());
			WorldBound.SetRadius(radius);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::SetSizeChangeReAdjustCamera(bool doReAdjust)
{
	mIsSizeChangeReAdjustCamera = doReAdjust;
}
//----------------------------------------------------------------------------
void Canvas::SetScreenSize(const Sizef &size)
{
	mScreenSize = size;

	if (mScreenSize.Width <= 0.0f)
		mScreenSize.Width = 1.0f;
	if (mScreenSize.Height <= 0.0f)
		mScreenSize.Height = 1.0f;
}
//----------------------------------------------------------------------------
void Canvas::SetViewPort(const Rectf &viewPort)
{
	mViewPort = viewPort;
}
//----------------------------------------------------------------------------
void Canvas::SetViewPort(float left, float bottom, float width, 
	float height)
{
	mViewPort.Left = left;
	mViewPort.Bottom = bottom;
	mViewPort.Right = left + width;
	mViewPort.Top = bottom + height;
}
//----------------------------------------------------------------------------
void Canvas::AddCamera(Camera *camera)
{
	std::map<Camera*, CullerPtr>::iterator it = mCullers.find(camera);
	if (it == mCullers.end())
	{
		mCameras.push_back(camera);
		Culler *culler = new0 Culler();
		culler->SetCamera(camera);

		mCullers[camera] = culler;
	}
}
//----------------------------------------------------------------------------
void Canvas::RemoveCamera(Camera *camera)
{
	std::vector<Camera *>::iterator itVect = std::find(mCameras.begin(),
		mCameras.end(), camera);
	if (itVect != mCameras.end())
	{
		mCameras.erase(itVect);
	}

	std::map<Camera*, CullerPtr>::iterator it = mCullers.find(camera);
	if (it != mCullers.end())
	{
		mCullers.erase(it);
	}
}
//----------------------------------------------------------------------------
void Canvas::RemoveAllCameras()
{
	mCameras.clear();
	mCullers.clear();
}
//----------------------------------------------------------------------------
void Canvas::OnGetVisibleSet(Culler& culler, bool noCull)
{
	Movable *cullingScene = culler.GetCurCullingScene();
	if (cullingScene == this)
	{ 
		// 从当前canvas开始，根

		std::vector<MovablePtr>::iterator iter = mChild.begin();
		std::vector<MovablePtr>::iterator end = mChild.end();
		for (/**/; iter != end; ++iter)
		{
			Movable* child = *iter;
			if (child && child != mCanvasRenderBind)
			{
				child->GetVisibleSet(culler, noCull);
			}
		}

		if (mRenderNode)
		{
			mRenderNode->GetVisibleSet(culler, noCull);
		}
	}
	else
	{
		// 内部canvas, 只把canvas的的CanvasRenderBind加入

		mCanvasRenderBind->GetVisibleSet(culler, noCull);

		ClearVisibleSet();
		ComputeVisibleSet();
	}
}
//----------------------------------------------------------------------------
//bool Canvas::GetPickRay(float viewPortPosX, float viewPortPosZ,
//	APoint& origin, AVector& direction)
//{
//	if (!mCamera) return false;
//
//	// Get the [0,1]^2-normalized coordinates of (x,y).
//	float r = viewPortPosX / (float)mSize.Width;
//	float u = viewPortPosZ / (float)mSize.Height;
//
//	// Get the relative coordinates in [rmin,rmax]x[umin,umax].
//	float rBlend = (1.0f - r)*mCamera->GetRMin() + r*mCamera->GetRMax();
//	float uBlend = (1.0f - u)*mCamera->GetUMin() + u*mCamera->GetUMax();
//
//	if (mCamera->IsPerspective())
//	{
//		origin = mCamera->GetPosition();
//		direction = mCamera->GetDMin()*mCamera->GetDVector() +
//			rBlend*mCamera->GetRVector() + uBlend*mCamera->GetUVector();
//		direction.Normalize();
//	}
//	else
//	{
//		origin = mCamera->GetPosition() + rBlend*mCamera->GetRVector() +
//			uBlend*mCamera->GetUVector();
//		direction = mCamera->GetDVector();
//		direction.Normalize();
//	}
//
//	return true;
//}
//----------------------------------------------------------------------------
//Vector2f Canvas::WorldPos3DToViewPort(const APoint &worldPos,
//	bool *isInBack)
//{
//	Rectf viewPort = mViewPort;
//	if (viewPort.IsEmpty())
//		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);
//
//	HMatrix matProjView = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix();
//	HPoint hPoint(worldPos.X(), worldPos.Y(), worldPos.Z(), worldPos.W());
//	HPoint tempPoint = matProjView * hPoint;
//
//	if (isInBack)
//	{
//		if (tempPoint.Z() <= 0)
//			*isInBack = true;
//		else
//			*isInBack = false;
//	}
//
//	float wInv = 1.0f / tempPoint.W();
//
//	//投影坐标范围为[-1,1]要变成[0,1]
//	Vector2f viewPortPos;
//	viewPortPos.X() = (1.0f + tempPoint.X()*wInv) / 2.0f;
//	viewPortPos.Y() = (1.0f + tempPoint.Y()*wInv) / 2.0f;
//
//	//投影坐标范围为[0,1]要变成视口内坐标
//	viewPortPos.X() = viewPort.Left + viewPortPos.X()*viewPort.Width();
//	viewPortPos.Y() = viewPort.Bottom + viewPortPos.Y()*viewPort.Height();
//
//	return viewPortPos;
//}
//----------------------------------------------------------------------------
//AVector Canvas::ScreenVectorToCamera(const AVector &screenVec)
//{
//	AVector camVec = screenVec;
//
//	if (mCamera->IsPerspective())
//	{
//		float dMin = 0.0f;
//		float dMax = 0.0f;
//		float uMin = 0.0f;
//		float uMax = 0.0f;
//		float rMin = 0.0f;
//		float rMax = 0.0f;
//		mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);
//
//		camVec = AVector(rMax*2.0f * screenVec.X() / mSize.Width,
//			screenVec.Y(), uMax*2.0f * screenVec.Z() / mSize.Height);
//	}
//
//	return camVec;
//}
//----------------------------------------------------------------------------
void Canvas::ClearVisibleSet()
{
	std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
	for (; it != mCullers.end(); it++)
	{
		it->second->Clear();
	}
}
//----------------------------------------------------------------------------
void Canvas::ComputeVisibleSet()
{
	if (!IsShow()) return;

	std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
	for (; it != mCullers.end(); it++)
	{
		it->second->Clear();
		it->second->ComputeVisibleSet(this);
		it->second->GetVisibleSet().Sort();	
	}
}
//----------------------------------------------------------------------------
void Canvas::Draw(Renderer *renderer)
{
	if (!IsShow()) return;

	if (GetParent())
	{
		APoint worldPos = WorldTransform.GetTranslate();
		APoint leftPos = worldPos + LeftBottomCornerOffset;
		SetViewPort(leftPos.X(), leftPos.Z(), mSize.Width, mSize.Height);
	}

	// before
	renderer->InitRenderStates();

	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty()) viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);
	renderer->SetViewport(viewPort);
	renderer->ClearDepthBuffer();

	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		Camera *camera = mCameras[i];
		Culler *culler = mCullers[camera];

		VisibleSet &visibleSet = culler->GetVisibleSet();

		renderer->SetCamera(camera);
		PX2_GR.GetCurEnvirParam()->ComputeEnvironment(visibleSet);
		renderer->Draw(visibleSet);
	}

	renderer->ClearDepthBuffer();
	for (int i = 0; i < (int)mCameras.size(); i++)
	{
		Camera *camera = mCameras[i];
		renderer->SetCamera(camera);

		renderer->Update(GetDebugLine()->GetVertexBuffer());
		renderer->Draw(GetDebugLine());
	}
}
//----------------------------------------------------------------------------
bool Canvas::LessThan(const Canvas *step0, const Canvas *step1)
{
	int priority0 = step0->GetPriority();
	int priority1 = step1->GetPriority();

	if (priority0 == priority1)
		return step0 < step1;

	return priority0 > priority1;
}
//----------------------------------------------------------------------------
void Canvas::SetPickOnlyInSizeRange(bool inRange)
{
	mIsPickOnlyInSizeRange = inRange;
}
//----------------------------------------------------------------------------
void Canvas::DoExecute(Event *event)
{
	if (InputEventSpace::IsIn(event))
	{
		// get data
		InputEventData data = event->GetData<InputEventData>();
		//if (data.ViewID != id)
		//	return;

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
			OnMouseWheel(worldPos, data.MWheel);
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::OnLeftDown(const APoint &worldPos)
{
	mIsPressed = true;
	mIsLeftPressed = true;
	mIsMoved = false;

	mPressedPos = worldPos;

	mLastPickPos = mCurPickPos;
	mCurPickPos = mPressedPos;

	mLeftPressedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnLeftUp(const APoint &worldPos)
{
	mIsPressed = false;
	mIsLeftPressed = false;
	mIsMoved = false;

	mLastPickPos = mCurPickPos;
	mCurPickPos = mReleasedPos;

	mLeftReleasedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnLeftDClick(const APoint &worldPos)
{
	PX2_UNUSED(worldPos);
}
//----------------------------------------------------------------------------
void Canvas::OnMiddleDown(const APoint &worldPos)
{
	mIsPressed = true;
	mIsMiddlePressed = true;
	mIsMoved = false;

	mPressedPos = worldPos;

	mLastPickPos = mCurPickPos;
	mCurPickPos = worldPos;

	mMiddlePressedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnMiddleUp(const APoint &worldPos)
{
	mIsPressed = false;
	mIsMiddlePressed = false;
	mIsMoved = false;

	mLastPickPos = mCurPickPos;
	mCurPickPos = worldPos;

	mMiddleReleasedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnMouseWheel(const APoint &worldPos, float delta)
{
	PX2_UNUSED(worldPos);
	PX2_UNUSED(delta);
}
//----------------------------------------------------------------------------
void Canvas::OnRightDown(const APoint &worldPos)
{
	mIsPressed = true;
	mIsRightPressed = true;
	mIsMoved = false;

	mPressedPos = worldPos;

	mLastPickPos = mCurPickPos;
	mCurPickPos = worldPos;

	mRightPressedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnRightUp(const APoint &worldPos)
{
	mIsPressed = false;
	mIsRightPressed = false;
	mIsMoved = false;

	mLastPickPos = mCurPickPos;
	mCurPickPos = worldPos;

	mRightReleasedPos = worldPos;
}
//----------------------------------------------------------------------------
void Canvas::OnMotion(const APoint &worldPos)
{
	mMoveDelta = worldPos - mCurPickPos;

	mLastPickPos = mCurPickPos;
	mCurPickPos = worldPos;

	mIsMoved = true;
}
//----------------------------------------------------------------------------
std::pair<float, float> Canvas::CalPixelToWorld()
{
	//Rectf viewPort = mViewPort;
	//if (viewPort.IsEmpty())
	//	viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	//std::pair<float, float> pixelToWorld;

	//if (mCamera)
	//{
	//	if (mCamera->IsPerspective())
	//	{
	//		float rMin = mCamera->GetRMin();
	//		float uMin = mCamera->GetUMin();
	//		float viewPortWidth = viewPort.Width();
	//		float viewPortHeight = viewPort.Height();

	//		float worldW = 2.0f * -rMin;
	//		float worldH = 2.0f * -uMin;

	//		worldW *= 10.0f;
	//		worldH *= 10.0f;

	//		pixelToWorld.first = worldW / (float)viewPortWidth;
	//		pixelToWorld.second = worldH / (float)viewPortHeight;
	//	}
	//	else
	//	{
	//		float rMin = mCamera->GetRMin();
	//		float uMin = mCamera->GetUMin();
	//		float viewPortWidth = viewPort.Width();
	//		float viewPortHeight = viewPort.Height();

	//		float worldW = 2.0f * -rMin;
	//		float worldH = 2.0f * -uMin;

	//		worldW *= 1.0f;
	//		worldH *= 1.0f;

	//		pixelToWorld.first = worldW / (float)viewPortWidth;
	//		pixelToWorld.second = worldH / (float)viewPortHeight;
	//	}

	//}

	//mPixelToWorld = pixelToWorld;

	return mPixelToWorld;
}
//----------------------------------------------------------------------------
void Canvas::ClearDebugLine()
{
	mDebugPoly->SetNumSegments(0);
}
//----------------------------------------------------------------------------
void Canvas::AddDebugLine(const APoint &fromPos, const APoint &toPos,
	const Float4 &color)
{
	int numPoly = mDebugPoly->GetNumSegments();

	VertexBufferAccessor vba(mDebugPoly);

	int index = numPoly * 2;
	vba.Position<Float3>(index) = fromPos;
	vba.Color<Float4>(0, index) = color;
	vba.Position<Float3>(index + 1) = toPos;
	vba.Color<Float4>(0, index + 1) = color;

	mDebugPoly->SetNumSegments(numPoly + 1);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Canvas::Canvas(LoadConstructor value) :
SizeNode(value),
mRenderNode(0),
mRenderWindow(0),
mIsMain(false),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mIsPickOnlyInSizeRange(true),
mIsInRange(false),
mIsPressed(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false)
{
}
//----------------------------------------------------------------------------
void Canvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	SizeNode::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(Canvas, source);
}
//----------------------------------------------------------------------------
void Canvas::Link(InStream& source)
{
	SizeNode::Link(source);
}
//----------------------------------------------------------------------------
void Canvas::PostLink()
{
	SizeNode::PostLink();
}
//----------------------------------------------------------------------------
bool Canvas::Register(OutStream& target) const
{
	if (SizeNode::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Canvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	SizeNode::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Canvas, target);
}
//----------------------------------------------------------------------------
int Canvas::GetStreamingSize(Stream &stream) const
{
	int size = SizeNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------