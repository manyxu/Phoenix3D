// PX2EU_BluePrintFrame.hpp

#ifndef PX2EU_BLUEPRINTFRAME_HPP
#define PX2EU_BLUEPRINTFRAME_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2EU_BPFrame.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_BluePrintFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_BluePrintFrame);

	public:
		EU_BluePrintFrame();
		virtual ~EU_BluePrintFrame();

		virtual void OnEvent(PX2::Event *event);

		EU_BPFrame *AddBPFrame(const std::string &name, UIFrame *bpObject);

	protected:
		UIFramePtr mToolFrame;

		UITabFramePtr mTableFrame;
		std::vector<EU_BPFramePtr> mBPFrames;
	};

	PX2_REGISTER_STREAM(EU_BluePrintFrame);
	typedef PointerRef<EU_BluePrintFrame> EU_BluePrintFramePtr;

}

#endif