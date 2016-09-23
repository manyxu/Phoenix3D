// PX2EU_InspectorFrame.hpp

#ifndef PX2EU_INSPECTORFRAME_HPP
#define PX2EU_INSPECTORFRAME_HPP

#include "PX2UIItem.hpp"
#include "PX2UIPropertyGrid.hpp"
#include "PX2EU_PropertyGrid.hpp"
#include "PX2EU_InspectorFrameRes.hpp"
#include "PX2EU_EditFrame.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_InspectorFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_InspectorFrame);

	public:
		EU_InspectorFrame();
		virtual ~EU_InspectorFrame();

		enum InspType
		{
			IT_PROP,
			IT_RES,
			IT_EDIT,
			IT_MAX_TYPE
		};
		void SetInspType(InspType type);
		InspType GetInspType() const;

	protected:
		void _PropUICallback(UIFrame *frame, UICallType type);

		InspType mInspType;
		UIFramePtr mToolFrame;
		EU_InspectorFrameResPtr mInspectorFrameRes;
		EU_PropertyGridPtr mPropertyGrid;
		EU_EditFramePtr mEditFrame;
	};

	PX2_REGISTER_STREAM(EU_InspectorFrame);
	typedef PointerRef<EU_InspectorFrame> EU_InspectorFramePtr;

}

#endif