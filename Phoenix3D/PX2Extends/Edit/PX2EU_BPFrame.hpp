// PX2EU_BPFrame.hpp

#ifndef PX2EU_BPFRAME_HPP
#define PX2EU_BPFRAME_HPP

#include "PX2EditPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_BPFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_BPFrame);

	public:
		EU_BPFrame();
		virtual ~EU_BPFrame();

		void CreateChildUICanvas();

	protected:
		UIFramePtr mToolFrame;
		UICanvasPtr mBPUICanvas;
	};

	PX2_REGISTER_STREAM(EU_BPFrame);
	typedef Pointer0<EU_BPFrame> EU_BPFramePtr;

}

#endif