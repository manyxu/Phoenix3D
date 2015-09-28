// PX2EU_InspectorFrame.hpp

#ifndef PX2EU_INSPECTORFRAME_HPP
#define PX2EU_INSPECTORFRAME_HPP

#include "PX2UIItem.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_InspectorFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_InspectorFrame);

	public:
		EU_InspectorFrame();
		virtual ~EU_InspectorFrame();

		static float sItemSize;

	protected:
		UIItemPtr mRootItem;
	};

	PX2_REGISTER_STREAM(EU_InspectorFrame);
	typedef Pointer0<EU_InspectorFrame> EU_InspectorFramePtr;

}

#endif