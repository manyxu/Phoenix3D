// PX2UIView.cpp

#include "PX2Canvas.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, Canvas);
PX2_IMPLEMENT_STREAM(Canvas);
PX2_IMPLEMENT_FACTORY(Canvas);
PX2_IMPLEMENT_DEFAULT_NAMES(SizeNode, Canvas);

//----------------------------------------------------------------------------
Canvas::Canvas() :
mRenderer(0),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mBeforeDoClearColor(false),
mBeforeDoClearDepth(false),
mBeforeDoClearStencil(false),
mAfterDoClearColor(false),
mAfterDoClearDepth(false),
mAfterDoClearStencil(false)
{
	mOverrideWireProperty = new0 WireProperty();
	mOverrideWireProperty->Enabled = true;

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Canvas::~Canvas()
{
	if (mRenderer && mRenderer->GetCamera() == mCamera)
	{
		mRenderer->SetCamera(0);
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
void Canvas::OnSizeChanged()
{
	if (mIsSizeChangeReAdjustCamera && mCamera)
	{
		if (mCamera->IsPerspective())
		{
			float fov = 0.0f;
			float asp = 1.0f;
			float dMin = 0.0f;
			float dMax = 0.0f;
			mCamera->GetFrustum(fov, asp, dMin, dMax);
			mCamera->SetFrustum(fov, mSize.Width / mSize.Height, dMin, dMax);
		}
	}

	SizeNode::OnSizeChanged();
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
void Canvas::SetRenderer(Renderer *renderer)
{
	mRenderer = renderer;
}
//----------------------------------------------------------------------------
void Canvas::SetCamera(Camera *camera)
{
	mCamera = camera;
	mCuller.SetCamera(mCamera);
}
//----------------------------------------------------------------------------
void Canvas::OnGetVisibleSet(Culler& culler, bool noCull)
{
	bool isInternalCanvas = culler.IsInternalCanvas();
	Movable *cullingScene = culler.GetCurCullingScene();
	
	if (!isInternalCanvas)
	{
		// top
		if (cullingScene == this)
		{
			std::vector<MovablePtr>::iterator iter = mChild.begin();
			std::vector<MovablePtr>::iterator end = mChild.end();
			for (/**/; iter != end; ++iter)
			{
				Movable* child = *iter;
				if (child)
				{
					child->GetVisibleSet(culler, noCull);
				}
			}
		}
		else
		{
			culler.GetVisibleCanvas().push_back(this);
			return;
		}
	}
	else
	{
		std::vector<MovablePtr>::iterator iter = mChild.begin();
		std::vector<MovablePtr>::iterator end = mChild.end();
		for (/**/; iter != end; ++iter)
		{
			Movable* child = *iter;
			if (child)
			{
				child->GetVisibleSet(culler, noCull);
			}
		}
	}
}
//----------------------------------------------------------------------------
bool Canvas::GetPickRay(float screenX, float screenY, APoint& origin, 
	AVector& direction)
{
	if (!mCamera) return false;

	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty())
		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	if (viewPort.IsEmpty()) return false;

	// Get the current viewport and test whether (x,y) is in it.
	float viewX = viewPort.Left;
	float viewY = viewPort.Bottom;
	float viewW = viewPort.Width();
	float viewH = viewPort.Height();

	// Get the [0,1]^2-normalized coordinates of (x,y).
	float r = ((float)(screenX - viewX)) / (float)viewW;
	float u = ((float)(screenY - viewY)) / (float)viewH;

	// Get the relative coordinates in [rmin,rmax]x[umin,umax].
	float rBlend = (1.0f - r)*mCamera->GetRMin() + r*mCamera->GetRMax();
	float uBlend = (1.0f - u)*mCamera->GetUMin() + u*mCamera->GetUMax();

	if (mCamera->IsPerspective())
	{
		origin = mCamera->GetPosition();
		direction = mCamera->GetDMin()*mCamera->GetDVector() +
			rBlend*mCamera->GetRVector() + uBlend*mCamera->GetUVector();
		direction.Normalize();
	}
	else
	{
		origin = mCamera->GetPosition() + rBlend*mCamera->GetRVector() +
			uBlend*mCamera->GetUVector();
		direction = mCamera->GetDVector();
		direction.Normalize();
	}

	return true;
}
//----------------------------------------------------------------------------
Vector2f Canvas::PointWorldToViewPort(const APoint &point,
	bool *isInBack)
{
	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty())
		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	HMatrix matProjView = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix();
	HPoint hPoint(point.X(), point.Y(), point.Z(), point.W());
	HPoint tempPoint = matProjView * hPoint;

	if (isInBack)
	{
		if (tempPoint.Z() <= 0)
			*isInBack = true;
		else
			*isInBack = false;
	}

	float wInv = 1.0f / tempPoint.W();

	//投影坐标范围为[-1,1]要变成[0,1]
	Vector2f screenPoint;
	screenPoint.X() = (1.0f + tempPoint.X()*wInv) / 2.0f;
	screenPoint.Y() = (1.0f + tempPoint.Y()*wInv) / 2.0f;

	//投影坐标范围为[0,1]要变成视口内坐标
	screenPoint.X() = viewPort.Left + screenPoint.X()*viewPort.Width();
	screenPoint.Y() = viewPort.Bottom + screenPoint.Y()*viewPort.Height();

	return screenPoint;
}
//----------------------------------------------------------------------------
AVector Canvas::ScreenVectorToCamera(const AVector &screenVec)
{
	AVector camVec = screenVec;

	if (mCamera->IsPerspective())
	{
		float dMin = 0.0f;
		float dMax = 0.0f;
		float uMin = 0.0f;
		float uMax = 0.0f;
		float rMin = 0.0f;
		float rMax = 0.0f;
		mCamera->GetFrustum(dMin, dMax, uMin, uMax, rMin, rMax);

		camVec = AVector(rMax*2.0f * screenVec.X() / mSize.Width,
			screenVec.Y(), uMax*2.0f * screenVec.Z() / mSize.Height);
	}

	return camVec;
}
//----------------------------------------------------------------------------
void Canvas::ClearVisibleSet()
{
	mCuller.Clear();
}
//----------------------------------------------------------------------------
void Canvas::ComputeVisibleSetAndEnv()
{
	if (!IsShow()) return;

	const Camera *cam = mCuller.GetCamera();
	if (cam)
	{
		mCuller.SetInternalCanvas(0 != GetParent());
		mCuller.ComputeVisibleSet(this);
		mCuller.GetVisibleSet().Sort();
	}
	else mCuller.Clear();
}
//----------------------------------------------------------------------------
void Canvas::Draw()
{
	if (!IsShow()) return;

	if (mRenderer)
	{
		CameraPtr beforeCamer = mRenderer->GetCamera();

		mRenderer->InitRenderStates();

		Rectf viewPort = mViewPort;
		if (viewPort.IsEmpty()) viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);
		mRenderer->SetViewport(viewPort);

		Float4 renderClearColor = mRenderer->GetClearColor();
		if (mBeforeDoClearColor)
		{
			mRenderer->SetClearColor(mClearColor);
			mRenderer->ClearColorBuffer();
		}

		if (mBeforeDoClearDepth)
			mRenderer->ClearDepthBuffer();

		if (mBeforeDoClearStencil)
			mRenderer->ClearStencilBuffer();

		mRenderer->SetCamera(mCamera);
		mRenderer->Draw(mCuller.GetVisibleSet());

		mRenderer->SetCamera(beforeCamer);
		mRenderer->SetClearColor(renderClearColor);

		if (mAfterDoClearColor)
			mRenderer->ClearColorBuffer();

		if (mAfterDoClearDepth)
			mRenderer->ClearDepthBuffer();

		if (mAfterDoClearStencil)
			mRenderer->ClearStencilBuffer();
	}
}
//----------------------------------------------------------------------------
void Canvas::SetBeforeDrawClear(bool color, bool depth, bool stencil)
{
	mBeforeDoClearColor = color;
	mBeforeDoClearDepth = depth;
	mBeforeDoClearStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::GetBeforeDrawClear(bool &color, bool &depth, bool &stencil)
{
	color = mBeforeDoClearColor;
	depth = mBeforeDoClearDepth;
	stencil = mBeforeDoClearStencil;
}
//----------------------------------------------------------------------------
void Canvas::SetAfterDrawClear(bool color, bool depth, bool stencil)
{
	mAfterDoClearColor = color;
	mAfterDoClearDepth = depth;
	mAfterDoClearStencil = stencil;
}
//----------------------------------------------------------------------------
void Canvas::GetAfterDrawClear(bool &color, bool &depth, bool &stencil)
{
	color = mAfterDoClearColor;
	depth = mAfterDoClearDepth;
	stencil = mAfterDoClearStencil;
}
//----------------------------------------------------------------------------
void Canvas::SetClearColor(const Float4 &clearColor)
{
	mClearColor = clearColor;
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
std::pair<float, float> Canvas::CalPixelToWorld()
{
	Rectf viewPort = mViewPort;
	if (viewPort.IsEmpty())
		viewPort = Rectf(0.0f, 0.0f, mSize.Width, mSize.Height);

	std::pair<float, float> pixelToWorld;

	if (mCamera)
	{
		if (mCamera->IsPerspective())
		{
			float rMin = mCamera->GetRMin();
			float uMin = mCamera->GetUMin();
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
			float rMin = mCamera->GetRMin();
			float uMin = mCamera->GetUMin();
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

	return pixelToWorld;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Canvas::Canvas(LoadConstructor value) :
SizeNode(value),
mRenderer(0),
mIsSizeChangeReAdjustCamera(true),
mPriority(0),
mBeforeDoClearColor(false),
mBeforeDoClearDepth(false),
mBeforeDoClearStencil(false),
mAfterDoClearColor(false),
mAfterDoClearDepth(false),
mAfterDoClearStencil(false)
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