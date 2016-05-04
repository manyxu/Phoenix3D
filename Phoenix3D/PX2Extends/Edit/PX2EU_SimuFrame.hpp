// PX2EU_SimuFrame.hpp

#ifndef PX2EU_SIMUFRAME_HPP
#define PX2EU_SIMUFRAME_HPP

#include "PX2EditPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2EU_SimuFrame.hpp"
#include "PX2EU_CanvasStageSimu.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_SimuFrame : public UIFrame, public Visitor
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_SimuFrame);

	public:
		EU_SimuFrame();
		virtual ~EU_SimuFrame();

		virtual void Visit(Object *obj, int info);

	protected:
		UIFramePtr mToolFrame;
		EU_CanvasStageSimuPtr mCanvasStageSimu;
	};

	PX2_REGISTER_STREAM(EU_SimuFrame);
	typedef PointerRef<EU_SimuFrame> EU_SimuFramePtr;

}

#endif