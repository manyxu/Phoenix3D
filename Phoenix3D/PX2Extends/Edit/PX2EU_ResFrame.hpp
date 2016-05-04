// PX2EU_ResFrame.hpp

#ifndef PX2EU_RESFRAME_HPP
#define PX2EU_RESFRAME_HPP

#include "PX2EU_ResTree.hpp"
#include "PX2EU_ResList.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UICanvas.hpp"
#include "PX2EU_GridFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ResFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResFrame);

	public:
		EU_ResFrame();
		virtual ~EU_ResFrame();

		void _SpliterDragingCallback(UIFrame *frame, UICallType type);

		virtual void OnSizeChanged();

	protected:
		void _SetFramePos();

		UIFramePtr mToolFrame;
		UICanvasPtr mTreeCanvas;
		EU_ResTreePtr mTreeDirs;
		UICanvasPtr mResCanvas;
		EU_ResListPtr mResList;
		EU_GridFramePtr mResGrid;
		UISplitterFramePtr mSplitter;
	};

	PX2_REGISTER_STREAM(EU_ResFrame);
	typedef PointerRef<EU_ResFrame> EU_ResFramePtr;

}

#endif