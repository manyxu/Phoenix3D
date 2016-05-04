// PX2EU_CanvasStageSimu.cpp

#include "PX2EU_CanvasStageSimu.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2InputEventData.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Project.hpp"
#include "PX2SimulationEventType.hpp"
#include "PX2Edit.hpp"
#include "PX2ActorPicker.hpp"
#include "PX2EditEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EngineCanvas, EU_CanvasStageSimu);
PX2_IMPLEMENT_STREAM(EU_CanvasStageSimu);
PX2_IMPLEMENT_FACTORY(EU_CanvasStageSimu);
PX2_IMPLEMENT_DEFAULT_STREAM(EngineCanvas, EU_CanvasStageSimu);

//----------------------------------------------------------------------------
EU_CanvasStageSimu::EU_CanvasStageSimu()
{
	SetCanvasAddType(EngineCanvas::CAT_SCENE);
	SetCanvasAddMode(EngineCanvas::CAM_SET_RENDERMOVABLE);

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetSimu(true);
	SetActivateSelfCtrled(false);
}
//----------------------------------------------------------------------------
EU_CanvasStageSimu::~EU_CanvasStageSimu()
{
}
//----------------------------------------------------------------------------
void EU_CanvasStageSimu::DoExecute(Event *event)
{
	EngineCanvas::DoExecute(event);
}
//----------------------------------------------------------------------------