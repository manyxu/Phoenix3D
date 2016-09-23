// PX2EU_CanvasStageSimu.hpp

#ifndef PX2EU_CANVASSTAGESIMU_HPP
#define PX2EU_CANVASSTAGESIMU_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CanvasStageSimu : public UICanvas
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasStageSimu);

	public:
		EU_CanvasStageSimu();
		virtual ~EU_CanvasStageSimu();
	};

	PX2_REGISTER_STREAM(EU_CanvasStageSimu);
	typedef PointerRef<EU_CanvasStageSimu> EU_CanvasStageSimuPtr;

}

#endif