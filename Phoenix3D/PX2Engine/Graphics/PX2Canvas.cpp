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
mIsRenderNodeUpdate(false),
mRenderWindow(0),
mIsMain(false),
mOverCamera(0),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mIsPickOnlyInSizeRange(true),
mIsDrawSetViewPort(true),
mClearStencil(0),
mClearFlagColor(false),
mClearFlagDepth(false),
mClearFlagStencil(false),
mIsPressed(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false),
mClearDepth(1.0f),
mIsPickedSomething(false)
{
	mCanvasRenderBind = new0 CanvasRenderBind();
	AttachChild(mCanvasRenderBind);

	mOverrideWireProperty = new0 WireProperty();
	mOverrideWireProperty->Enabled = true;

	mPixelToWorld.first = 0.0f;
	mPixelToWorld.second = 0.0f;

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

	SetActivateSelfCtrled(false);
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
void Canvas::SetRenderNode(Node *movable)
{
	mRenderNode = movable;
}
//----------------------------------------------------------------------------
void Canvas::SetRenderWindow(RenderWindow *rw)
{
	if (mRenderWindow)
	{
		mRenderWindow->RemoveCanvas(this);
	}

	mRenderWindow = rw;

	if (mRenderWindow)
	{
		mRenderWindow->AddCanvas(this);
	}
}
//----------------------------------------------------------------------------
void Canvas::OnSizeChanged()
{
	SizeNode::OnSizeChanged();

	if (GetParent())
	{
		Rectf worldRect = GetWorldRect();
		SetViewPort(worldRect);
	}

	CalPixelToWorld();
}
//----------------------------------------------------------------------------
void Canvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	CameraPtr beforeUpdateCamera = PX2_GR.GetCurUpdateCamera();

	if (mOverCamera)
	{
		PX2_GR.SetCurUpdateCamera(mOverCamera);
	}
	else
	{
		if (mCameras.size() > 0)
		{
			PX2_GR.SetCurUpdateCamera(mCameras[0]);
		}
	}

	ClearDebugLine();

	bool isLayoutChange = mIsLayoutChanged;
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

		mIsLayoutChanged = false;
	}

	SizeNode::UpdateWorldData(applicationTime, elapsedTime);

	if (mRenderNode && mIsRenderNodeUpdate)
	{
		mRenderNode->Update(applicationTime, elapsedTime);
	}

	PX2_GR.SetCurUpdateCamera(beforeUpdateCamera);
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
void Canvas::SetOverCamera(Camera *overCamera)
{
	if (mOverCamera)
	{
		mOverCameraCuller = 0;
	}

	mOverCamera = overCamera;

	if (mOverCamera)
	{
		mOverCameraCuller = new0 Culler();
		mOverCameraCuller->SetCamera(mOverCamera);
	}
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
void Canvas::SetClearColor(const Float4 &color)
{
	mClearColor = color;
}
//----------------------------------------------------------------------------
void Canvas::SetClearDepth(float depth)
{
	mClearDepth = depth;
}
//----------------------------------------------------------------------------
void Canvas::SetClearStencil(unsigned int stencil)
{
	mClearStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::SetClearFlag(bool color, bool depth, bool stencil)
{
	mClearFlagColor = color;
	mClearFlagDepth = depth;
	mClearFlagStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::ClearVisibleSet()
{
	if (mOverCamera)
	{
		mOverCameraCuller->Clear();
	}
	else
	{
		std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
		for (; it != mCullers.end(); it++)
		{
			it->second->Clear();
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::ComputeVisibleSet()
{
	if (!IsShow()) return;

	if (mOverCamera)
	{
		Movable *cullMov = this;
		if (mRenderNode)
			cullMov = mRenderNode;
		mOverCameraCuller->Clear();
		mOverCameraCuller->ComputeVisibleSet(cullMov);
		mOverCameraCuller->GetVisibleSet().Sort();
	}
	else
	{
		Movable *cullMov = this;
		if (mRenderNode)
			cullMov = mRenderNode;

		std::map<Camera*, CullerPtr>::iterator it = mCullers.begin();
		for (; it != mCullers.end(); it++)
		{
			it->second->Clear();
			it->second->ComputeVisibleSet(cullMov);
			it->second->GetVisibleSet().Sort();
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::Draw(Renderer *renderer)
{
	if (!IsShow()) return;

	if (GetParent())
	{
		const Rectf &worldRect = GetWorldRect();
		SetViewPort(worldRect);
	}

	// before
	Rectf beforeViewPort = renderer->GetViewPort();
	renderer->InitRenderStates();
	if (mIsDrawSetViewPort)
	{
		Rectf viewPort = mViewPort;
		if (viewPort.IsEmpty()) viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);
		renderer->SetViewport(viewPort);
	}

	bool bClearColor = false;
	bool bClearDepth = false;
	bool bClearStencil = false;
	GetClearFlag(bClearColor, bClearDepth, bClearStencil);
	_Clear(renderer, bClearColor, GetClearColor(),
		bClearDepth, GetClearDepth(), bClearStencil, GetClearStencil());

	if (mOverCamera)
	{
		_Draw(mOverCamera, renderer, mOverCameraCuller);
	}
	else
	{
		for (int i = 0; i < (int)mCameras.size(); i++)
		{
			Camera *camera = mCameras[i];
			Culler *culler = mCullers[camera];

			_Draw(camera, renderer, culler);
		}
	}

	renderer->SetViewport(beforeViewPort);
}
//----------------------------------------------------------------------------
void Canvas::_Clear(Renderer *renderer, bool bColor, const Float4 &color,
	bool bDepth, float depth, bool bStencil, int stencil)
{
	if (bColor && bDepth && bStencil)
	{
		renderer->SetClearColor(color);
		renderer->ClearBuffers();
	}
	else
	{
		if (bColor)
		{
			renderer->SetClearColor(color);
			renderer->ClearColorBuffer();
		}
		if (bDepth)
		{
			renderer->SetClearDepth(depth);
			renderer->ClearDepthBuffer();
		}
		if (bStencil)
		{
			renderer->SetClearStencil(stencil);
			renderer->ClearStencilBuffer();
		}
	}
}
//----------------------------------------------------------------------------
void Canvas::_Draw(Camera *camera, Renderer *renderer, Culler *culler)
{
	VisibleSet &visibleSet = culler->GetVisibleSet();

	CameraPtr beforeCamera = renderer->GetCamera();

	renderer->SetCamera(camera);
	EnvirParamController *evrParam = PX2_GR.GetCurEnvirParamController();
	if (evrParam)
		evrParam->ComputeEnvironment(visibleSet);

	bool clearColor = false;
	bool clearDepth = false;
	bool clearStencil = false;
	camera->GetClearFlag(clearColor, clearDepth, clearStencil);

	_Clear(renderer, clearColor,camera->GetClearColor(),
		clearDepth, camera->GetClearDepth(),
		clearStencil, camera->GetClearStencil());

	renderer->Draw(visibleSet);

	renderer->SetCamera(beforeCamera);
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
void Canvas::OnEvent(Event *event)
{
	PX2_UNUSED(event);
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
	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty())
		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	std::pair<float, float> pixelToWorld;

	if (mOverCamera)
	{
		if (mOverCamera->IsPerspective())
		{
			float rMin = mOverCamera->GetRMin();
			float uMin = mOverCamera->GetUMin();
			float viewPortWidth = viewPort.Width();
			float viewPortHeight = viewPort.Height();

			float worldW = 2.0f * -rMin;
			float worldH = 2.0f * -uMin;

			worldW *= 10.0f;
			worldH *= 10.0f;

			pixelToWorld.first = worldW / (float)viewPortWidth;
			pixelToWorld.second = worldH / (float)viewPortHeight;
		}
		else
		{
			float rMin = mOverCamera->GetRMin();
			float uMin = mOverCamera->GetUMin();
			float viewPortWidth = viewPort.Width();
			float viewPortHeight = viewPort.Height();

			float worldW = 2.0f * -rMin;
			float worldH = 2.0f * -uMin;

			worldW *= 1.0f;
			worldH *= 1.0f;

			pixelToWorld.first = worldW / (float)viewPortWidth;
			pixelToWorld.second = worldH / (float)viewPortHeight;
		}

	}

	mPixelToWorld = pixelToWorld;

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
mIsRenderNodeUpdate(false),
mIsMain(false),
mRenderNode(0),
mOverCamera(0),
mRenderWindow(0),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mIsPickOnlyInSizeRange(true),
mIsDrawSetViewPort(true),
mClearStencil(0),
mClearFlagColor(false),
mClearFlagDepth(false),
mClearFlagStencil(false),
mIsPressed(false),
mIsLeftPressed(false),
mIsRightPressed(false),
mIsMiddlePressed(false),
mIsMoved(false),
mIsPickedSomething(false)
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