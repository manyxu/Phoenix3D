// PX2EU_StageFrame.hpp

#ifndef PX2EU_STAGEFRAME_HPP
#define PX2EU_STAGEFRAME_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2EU_BPFrame.hpp"
#include "PX2EU_CanvasStage.hpp"
#include "PX2EU_CanvasStageUI.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_StageFrame : public UIFrame, public Visitor
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_StageFrame);

	public:
		EU_StageFrame();
		virtual ~EU_StageFrame();

		virtual void Visit(Object *obj, int info);

	protected:
		void AddStage();

		UIFramePtr mToolFrame;
		EU_CanvasStagePtr mCanvasStage;
		EU_CanvasStageUIPtr mCanvasStageUI;
	};

	PX2_REGISTER_STREAM(EU_StageFrame);
	typedef PointerRef<EU_StageFrame> EU_StageFramePtr;

}

#endif