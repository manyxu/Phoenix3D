// PX2EU_CanvasStageSimu.hpp

#ifndef PX2EU_CANVASSTAGESIMU_HPP
#define PX2EU_CANVASSTAGESIMU_HPP

#include "PX2EditPre.hpp"
#include "PX2UICanvas.hpp"
#include "PX2EngineCanvas.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_CanvasStageSimu : public EngineCanvas
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CanvasStageSimu);

	public:
		EU_CanvasStageSimu();
		virtual ~EU_CanvasStageSimu();

	public:
		virtual void DoExecute(Event *event);
	};

	PX2_REGISTER_STREAM(EU_CanvasStageSimu);
	typedef PointerRef<EU_CanvasStageSimu> EU_CanvasStageSimuPtr;

}

#endif