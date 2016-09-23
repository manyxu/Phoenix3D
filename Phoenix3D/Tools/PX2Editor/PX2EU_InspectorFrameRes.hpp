// PX2EU_InspectorFrameRes.hpp

#ifndef PX2EU_INSPECTORFRAMERES_HPP
#define PX2EU_INSPECTORFRAMERES_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2EU_PropertyGridRes.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UISizeExtendControl.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_InspectorFrameRes : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_InspectorFrameRes);

	public:
		EU_InspectorFrameRes();
		virtual ~EU_InspectorFrameRes();

		virtual void OnEvent(Event *event);

	protected:
		EU_PropertyGridResPtr mPropertyGridRes;
		UIFramePtr mShowFrame;
		UICanvasPtr mShowCanvas;
		UIFPicBoxPtr mShowPicBox;
		UISizeExtendControlPtr mUISizeExtendControl;
	};

	PX2_REGISTER_STREAM(EU_InspectorFrameRes);
	typedef PointerRef<EU_InspectorFrameRes> EU_InspectorFrameResPtr;

}

#endif