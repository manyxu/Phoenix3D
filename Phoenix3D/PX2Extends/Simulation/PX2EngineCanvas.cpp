// PX2EngineCanvas.cpp

#include "PX2EngineCanvas.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2SimulationEventData.hpp"
#include "PX2Project.hpp"
#include "PX2UIFPicBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UICanvas, EngineCanvas);
PX2_IMPLEMENT_STREAM(EngineCanvas);
PX2_IMPLEMENT_FACTORY(EngineCanvas);

//----------------------------------------------------------------------------
EngineCanvas::EngineCanvas() :
mCanvasAddMode(CAM_ATTACHCHILD),
mCanvasAddType(CAT_SCENEUI),
mIsSimu(false)
{
	ComeInEventWorld();

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);
}
//----------------------------------------------------------------------------
EngineCanvas::~EngineCanvas()
{
	mSceneCanvas = 0;
	mUICanvas = 0;
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EngineCanvas::SetCanvasAddMode(CanvasAddMode mode)
{
	mCanvasAddMode = mode;
}
//----------------------------------------------------------------------------
EngineCanvas::CanvasAddMode EngineCanvas::GetCanvasAddMode() const
{
	return mCanvasAddMode;
}
//----------------------------------------------------------------------------
void EngineCanvas::SetCanvasAddType(CanvasAddType cat)
{
	mCanvasAddType = cat;
}
//----------------------------------------------------------------------------
EngineCanvas::CanvasAddType EngineCanvas::GetCanvasAddType() const
{
	return mCanvasAddType;
}
//----------------------------------------------------------------------------
void EngineCanvas::SetSimu(bool simu)
{
	mIsSimu = simu;
}
//----------------------------------------------------------------------------
bool EngineCanvas::IsSimu() const
{
	return mIsSimu;
}
//----------------------------------------------------------------------------
void EngineCanvas::DoExecute(Event *event)
{
	UICanvas::DoExecute(event);

	if (SimuES::IsEqual(event, SimuES::NewProject) ||
		SimuES::IsEqual(event, SimuES::LoadedProject))
	{

	}
	else if (SimuES::IsEqual(event, SimuES::CloseProject))
	{

	}
	else if (SimuES::IsEqual(event, SimuES::NewScene))
	{
		if (CAT_SCENEUI == mCanvasAddType || CAT_SCENE == mCanvasAddType)
		{
			SceneCanvas *sceneCanvas = PX2_PROJ.GetSceneCanvas();

			if (CAM_ATTACHCHILD == mCanvasAddMode)
				AttachChild(sceneCanvas);
			else if (CAM_SET_RENDERMOVABLE == mCanvasAddMode)
				SetRenderNode(sceneCanvas);

			Scene *scene = PX2_PROJ.GetScene();
			Camera *camera = scene->GetUseCameraActor()->GetCameraNode()->GetCamera();

			sceneCanvas->SetCamera(camera);
			sceneCanvas->SetAnchorHor(0.0f, 1.0f);
			sceneCanvas->SetAnchorVer(0.0f, 1.0f);

			mSceneCanvas = sceneCanvas;
		}
	}
	else if (SimuES::IsEqual(event, SimuES::CloseScene))
	{
		if (mSceneCanvas)
		{
			if (CAM_ATTACHCHILD == mCanvasAddMode)
				DetachChild(mSceneCanvas);
			else if (CAM_SET_RENDERMOVABLE == mCanvasAddMode)
				SetRenderNode(0);

			mSceneCanvas = 0;
		}
	}
	else if (SimuES::IsEqual(event, SimuES::NewUI))
	{
		if (CAT_SCENEUI == mCanvasAddType || CAT_UI == mCanvasAddType)
		{
			UICanvas *uiCanvas = PX2_PROJ.GetUICanvas();

			if (CAM_ATTACHCHILD == mCanvasAddMode)
				AttachChild(uiCanvas);
			else if (CAM_SET_RENDERMOVABLE == mCanvasAddMode)
				SetRenderNode(uiCanvas);

			mUICanvas = uiCanvas;
		}
	}
	else if (SimuES::IsEqual(event, SimuES::CloseUI))
	{
		if (mUICanvas)
		{
			if (CAM_ATTACHCHILD == mCanvasAddMode)
				DetachChild(mUICanvas);
			else if (CAM_SET_RENDERMOVABLE == mCanvasAddMode)
				SetRenderNode(0);

			mUICanvas = 0;
		}
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnLeftDown(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_LEFT;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MousePressed);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnLeftUp(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_LEFT;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseReleased);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnLeftDClick(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_LEFT;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseDoublePressed);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnMiddleDown(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_MIDDLE;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MousePressed);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnMiddleUp(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_MIDDLE;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseReleased);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnMouseWheel(const APoint &worldPos, float delta)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_MIDDLE;
		sid.WorldPos = worldPos;
		sid.Wheel = delta;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseWheeled);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnRightDown(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_RIGHT;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MousePressed);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnRightUp(const APoint &worldPos)
{
	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_RIGHT;
		sid.WorldPos = worldPos;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseReleased);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnMotion(const APoint &worldPos)
{
	Canvas::OnMotion(worldPos);

	if (mIsSimu)
	{
		SimuInputData sid;
		sid.TheMouseTag = SimuInputData::MT_NONE;
		sid.WorldPos = worldPos;
		sid.MoveDelta = mMoveDelta;

		Event *ent = SimuES_I::CreateEventX(SimuES_I::MouseMoved);
		ent->SetData(sid);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EngineCanvas::EngineCanvas(LoadConstructor value) :
UICanvas(value),
mCanvasAddMode(CAM_ATTACHCHILD),
mCanvasAddType(CAT_SCENEUI),
mIsSimu(false)
{
}
//----------------------------------------------------------------------------
void EngineCanvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UICanvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EngineCanvas, source);
}
//----------------------------------------------------------------------------
void EngineCanvas::Link(InStream& source)
{
	UICanvas::Link(source);
}
//----------------------------------------------------------------------------
void EngineCanvas::PostLink()
{
	UICanvas::PostLink();
}
//----------------------------------------------------------------------------
bool EngineCanvas::Register(OutStream& target) const
{
	if (UICanvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineCanvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UICanvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EngineCanvas, target);
}
//----------------------------------------------------------------------------
int EngineCanvas::GetStreamingSize(Stream &stream) const
{
	int size = UICanvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------