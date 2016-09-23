// PX2EU_TimeLineFrame.hpp

#ifndef PX2EU_TIMELINEFRAME_HPP
#define PX2EU_TIMELINEFRAME_HPP

#include "PX2UIFrame.hpp"
#include "PX2UIList.hpp"
#include "PX2EU_CanvasTimeLineEdit.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_TimeLineFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_TimeLineFrame);

	public:
		EU_TimeLineFrame();
		virtual ~EU_TimeLineFrame();

	protected:
		void _TimeLineCallback(UIFrame *frame, UICallType type);

		UIFramePtr mToolFrame;
		EU_CanvasTimeLineEditPtr mCanvasTimeLineEdit;

	};

	PX2_REGISTER_STREAM(EU_TimeLineFrame);
	typedef PointerRef<EU_TimeLineFrame> EU_TimeLineFramePtr;

}

#endif