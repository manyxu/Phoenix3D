// PX2EU_CanvasStageSimu.cpp

#include "PX2EU_CanvasStageSimu.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EU_ProjectEventController.hpp"
#include "PX2InputEventData.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Project.hpp"
using namespace PX2;


PX2_IMPLEMENT_RTTI(PX2, UICanvas, EU_CanvasStageSimu);
PX2_IMPLEMENT_STREAM(EU_CanvasStageSimu);
PX2_IMPLEMENT_FACTORY(EU_CanvasStageSimu);
PX2_IMPLEMENT_DEFAULT_STREAM(UICanvas, EU_CanvasStageSimu);

//----------------------------------------------------------------------------
EU_CanvasStageSimu::EU_CanvasStageSimu()
{
	SetName("EU_CanvasStageSimu");

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetActivateSelfCtrled(false);

	SetClearFlag(true, true, true);
	SetClearColor(Float4::MakeColor(153, 217, 234, 255));
	mCamera->SetClearFlag(false, false, false);

	UICanvas *canvasSceneSimu = new0 UICanvas();
	AttachChild(canvasSceneSimu);
	canvasSceneSimu->SetName("SceneCanvasSimu");
	canvasSceneSimu->SetAnchorHor(0.0f, 1.0f);
	canvasSceneSimu->SetAnchorVer(0.0f, 1.0f);
	canvasSceneSimu->EnableAnchorLayout(true);
	canvasSceneSimu->AttachController(new0 EU_ProjectEventController());
	canvasSceneSimu->SetClearFlag(true, true, true);
	canvasSceneSimu->GetCameraNode()->GetCamera()->SetClearFlag(false, false, false);
	canvasSceneSimu->SetRenderNodeUpdate(false);
	canvasSceneSimu->Show(false);

	UICanvas *canvasUISimu = new0 UICanvas();
	AttachChild(canvasUISimu);
	canvasUISimu->LocalTransform.SetTranslateY(-1.0f);
	canvasUISimu->SetName("UICanvasSimu");
	canvasUISimu->SetAnchorHor(0.0f, 1.0f);
	canvasUISimu->SetAnchorVer(0.0f, 1.0f);
	canvasUISimu->EnableAnchorLayout(true);
	canvasUISimu->AttachController(new0 EU_ProjectEventController());
	canvasUISimu->SetClearFlag(false, true, true);
	canvasUISimu->GetCameraNode()->GetCamera()->SetClearFlag(false, false, false);
	canvasUISimu->SetRenderNodeUpdate(false);
	canvasUISimu->Show(false);
}
//----------------------------------------------------------------------------
EU_CanvasStageSimu::~EU_CanvasStageSimu()
{
}
//----------------------------------------------------------------------------