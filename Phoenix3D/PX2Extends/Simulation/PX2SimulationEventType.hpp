// PX2SimulationEventType.hpp

#ifndef PX2SIMULATIONEVENTTYPE_HPP
#define PX2SIMULATIONEVENTTYPE_HPP

#include "PX2SimulationPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_EXTENDS_ITEM SimuES)
	PX2_EVENT(NewProject)
	PX2_EVENT(LoadedProject)
	PX2_EVENT(SavedProject)
	PX2_EVENT(CloseProject)
	PX2_EVENT(NewScene)
	PX2_EVENT(SavedScene)
	PX2_EVENT(CloseScene)
	PX2_EVENT(NewUI)
	PX2_EVENT(CloseUI)
	PX2_EVENT(NewBP)
	PX2_EVENT(LoadedBP)
	PX2_EVENT(CloseBP)

	PX2_EVENT(Play)

	PX2_EVENT(ButReleased)

	PX2_EVENT(GeneralString)
	PX2_EVENT(Character_Die)
	PX2_EVENT(Actor_Delete)
	PX2_EVENT(Scene_ViewPortProjectChanged)
	PX2_EVENT(Scene_BloomChanged)
	PX2_EVENT(Scene_ShadowMapChange)
	PX2_DECLARE_EVENT_END(SimuES)

	PX2_DECLARE_EVENT_BEGIN(PX2_EXTENDS_ITEM SimuES_I)
	PX2_EVENT(EnterView)
	PX2_EVENT(LevelView)
	PX2_EVENT(KeyPressed)
	PX2_EVENT(KeyReleased)
	PX2_EVENT(MousePressed)
	PX2_EVENT(MouseDoublePressed)
	PX2_EVENT(MouseReleased)
	PX2_EVENT(MouseWheeled)
	PX2_EVENT(MouseMoved)
	PX2_EVENT(TouchPressed)
	PX2_EVENT(TouchReleased)
	PX2_EVENT(TouchMoved)
	PX2_EVENT(TouchCancelled)
	PX2_DECLARE_EVENT_END(SimuES_I);

	PX2_DECLARE_EVENT_BEGIN(PX2_EXTENDS_ITEM SimuES_E)
	PX2_EVENT(AddSelect)
	PX2_EVENT(RemoveSelect)
	PX2_EVENT(RemoveAllSelects)
	PX2_EVENT(PushUnDo)
	PX2_EVENT(UnDo)
	PX2_EVENT(ReDo)
	PX2_EVENT(NoUnDo)
	PX2_EVENT(NoReDo)
	PX2_EVENT(SetRadius)
	PX2_EVENT(BoundChanged)
	PX2_DECLARE_EVENT_END(SimuES_E)

}

#endif