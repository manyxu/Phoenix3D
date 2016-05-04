// PX2EditEventType.hpp

#ifndef PX2EDITEVENTTYPE_HPP
#define PX2EDITEVENTTYPE_HPP

#include "PX2EditPre.hpp"
#include "PX2EventSystem.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_EXTENDS_ITEM EditEventSpace)
	PX2_EVENT(N_AddMenu)
	PX2_EVENT(N_NewProject)
	PX2_EVENT(N_OpenProject)
	PX2_EVENT(N_SaveProject)
	PX2_EVENT(N_CloseProject)
	PX2_EVENT(N_NewScene)
	PX2_EVENT(N_OpenScene)
	PX2_EVENT(N_SaveScene)
	PX2_EVENT(N_SaveSceneAs)
	PX2_EVENT(N_CloseScene)
	PX2_EVENT(N_Exit)
	PX2_EVENT(N_Window)
	PX2_EVENT(N_Simu)
	PX2_EVENT(N_ObjectNameChanged)
	PX2_EVENT(N_PlayTip)

	PX2_EVENT(SelectResDir)
	PX2_EVENT(SelectRes)
	PX2_EVENT(SetPreViewObject)

	PX2_EVENT(ObjectTransformChanged)
	PX2_EVENT(ObjectSizeChanged)

	PX2_EVENT(SetEditType)
	PX2_EVENT(SetEditAxisMode)
	PX2_EVENT(SetEditMode)

	PX2_EVENT(SetTheme)
	
	PX2_EVENT(SetPickPos)

	PX2_EVENT(FindActorInProjectTree)
	PX2_EVENT(FindActorInResTree)

	PX2_EVENT(TimeLine_UIPosDrag)

	PX2_EVENT(SelectTerrainPage)

	PX2_EVENT(ToSelectRes)

	PX2_EVENT(TimeLine_FitHor)
	PX2_EVENT(TimeLine_FitVer)
	PX2_EVENT(TimeLine_Fit_Selected)
	PX2_EVENT(TimeLine_Pan)
	PX2_EVENT(TimeLine_Zoom)
	PX2_EVENT(TimeLine_CurveMode)
	PX2_EVENT(CurveChangedByPropertyGrid)

	PX2_DECLARE_EVENT_END(EditEventSpace)

}

#endif