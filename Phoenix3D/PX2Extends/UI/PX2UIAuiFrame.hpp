// PX2UIAuiFrame.hpp

#ifndef PX2UIAUIFRAME_HPP
#define PX2UIAUIFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIAuiBlockFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIAuiFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIAuiFrame);

	public:
		UIAuiFrame();
		virtual ~UIAuiFrame();

		UISplitterFramePtr SpLeft;
		UISplitterFramePtr SpRight;
		UISplitterFramePtr SpBottom;
		UISplitterFramePtr SpTop;
		UIAuiBlockFramePtr AuiBlockDefault;
	};

	PX2_REGISTER_STREAM(UIAuiFrame);
	typedef PointerRef<UIAuiFrame> UIAuiFramePtr;

}

#endif