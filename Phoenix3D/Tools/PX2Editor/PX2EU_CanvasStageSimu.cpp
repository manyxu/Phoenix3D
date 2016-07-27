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
	RemoveAllCameras();

	SetName("EU_CanvasStageSimu");

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetActivateSelfCtrled(false);

	AttachController(new0 EU_ProjectEventController());

	SetClearFlag(true, true, true);
	SetClearColor(Float4::MakeColor(153, 217, 234, 255));
	mCamera->SetClearFlag(false, false, false);
}
//----------------------------------------------------------------------------
EU_CanvasStageSimu::~EU_CanvasStageSimu()
{
}
//----------------------------------------------------------------------------