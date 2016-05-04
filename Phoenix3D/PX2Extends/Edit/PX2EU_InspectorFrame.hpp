// PX2EU_InspectorFrame.hpp

#ifndef PX2EU_INSPECTORFRAME_HPP
#define PX2EU_INSPECTORFRAME_HPP

#include "PX2UIItem.hpp"
#include "PX2UIPropertyGrid.hpp"
#include "PX2EU_PropertyGrid.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_InspectorFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_InspectorFrame);

	public:
		EU_InspectorFrame();
		virtual ~EU_InspectorFrame();

	protected:
		UIFramePtr mToolFrame;
		EU_PropertyGridPtr mPropertyGrid;
	};

	PX2_REGISTER_STREAM(EU_InspectorFrame);
	typedef PointerRef<EU_InspectorFrame> EU_InspectorFramePtr;

}

#endif