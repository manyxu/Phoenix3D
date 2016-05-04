// PX2EU_ResGridItem.hpp

#ifndef PX2EU_RESGRIDITEM_HPP
#define PX2EU_RESGRIDITEM_HPP

#include "PX2UIGridFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ResGridItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResGridItem);

	public:
		EU_ResGridItem();
		virtual ~EU_ResGridItem();

		void SetPathFilename(const std::string &pathFilename);

		static Sizef sGridItemSize;
		static UIFramePtr sLastSelectItemBut;

	protected:
		void _ButCallback(UIFrame *frame, UICallType type);

		UIButtonPtr mButton;
		UIFPicBoxPtr mResPicBox;
		UIFTextPtr mText;
	};

	PX2_REGISTER_STREAM(EU_ResGridItem);
	typedef PointerRef<EU_ResGridItem> EU_ResGridItemPtr;

}

#endif