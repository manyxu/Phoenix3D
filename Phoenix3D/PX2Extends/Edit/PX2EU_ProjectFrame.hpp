// PX2EU_Project.hpp

#ifndef PX2EU_PROJECTFRAME_HPP
#define PX2EU_PROJECTFRAME_HPP

#include "PX2UIItem.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ProjectFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectFrame);

	public:
		EU_ProjectFrame();
		virtual ~EU_ProjectFrame();

		static float sItemSize;

	protected:
		UIItemPtr mRootItem;
	};

	PX2_REGISTER_STREAM(EU_ProjectFrame);
	typedef Pointer0<EU_ProjectFrame> EU_ProjectFramePtr;

}

#endif