// PX2EU_GridFrame.hpp

#ifndef PX2EU_GRIDFRAME_HPP
#define PX2EU_GRIDFRAME_HPP

#include "PX2UIGridFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_GridFrame : public UIGridFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_GridFrame);

	public:
		EU_GridFrame();
		virtual ~EU_GridFrame();

		void DoExecute(Event *event);

	protected:
		void RefreshItems(const std::string &path);

		std::string mSelectPath;
	};

	PX2_REGISTER_STREAM(EU_GridFrame);
	typedef PointerRef<EU_GridFrame> EU_GridFramePtr;

}

#endif