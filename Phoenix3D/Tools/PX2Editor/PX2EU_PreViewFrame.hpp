// PX2EU_PreView.hpp

#ifndef EU_PREVIEWFRAME_HPP
#define EU_PREVIEWFRAME_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"
#include "PX2EU_CanvasPreView.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_PreViewFrame : public UIFrame, public Visitor
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_PreViewFrame);

	public:
		EU_PreViewFrame();
		virtual ~EU_PreViewFrame();

	protected:
		UIFramePtr mToolFrame;
		EU_CanvasPreViewPtr mCanvasPreView;
	};

	PX2_REGISTER_STREAM(EU_PreViewFrame);
	typedef PointerRef<EU_PreViewFrame> EU_PreViewFramePtr;

}

#endif