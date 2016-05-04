// PX2EU_CreateFrame.hpp

#ifndef PX2EU_CREATEFRAME_HPP
#define PX2EU_CREATEFRAME_HPP

#include "PX2EU_ProjectTree.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_CreateFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CreateFrame);

	public:
		EU_CreateFrame();
		virtual ~EU_CreateFrame();

	protected:
		UITabFramePtr mTableFrame;
	};

	PX2_REGISTER_STREAM(EU_CreateFrame);
	typedef PointerRef<EU_CreateFrame> EU_CreateFramePtr;

}

#endif