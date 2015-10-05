// PX2EU_ResFrame.hpp

#ifndef PX2EU_RESFRAME_HPP
#define PX2EU_RESFRAME_HPP

#include "PX2EU_ResTree.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ResFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResFrame);

	public:
		EU_ResFrame();
		virtual ~EU_ResFrame();

	protected:
		EU_ResTreePtr mTree;
	};

	PX2_REGISTER_STREAM(EU_ResFrame);
	typedef Pointer0<EU_ResFrame> EU_ResFramePtr;

}

#endif