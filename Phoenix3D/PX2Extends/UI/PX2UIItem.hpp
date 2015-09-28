// PX2UIItem.hpp

#ifndef PX2UIITEM_HPP
#define PX2UIITEM_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIText.hpp"

namespace PX2
{

	class UIItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIItem);

	public:
		UIItem();
		virtual ~UIItem();

		enum IconArrowState
		{
			IAS_NONE,
			IAS_ARROW0,
			IAS_ARROW1,
			IAS_MAX_STATE
		};
		void SetIconArrowState(IconArrowState state);
		IconArrowState GetIconArrowState() const;

		UIPicBox *GetIconArrow0();
		UIPicBox *GetIconArrow1();
		UIPicBox *GetIcon();
		UIText *GetText();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _Recal();

		bool mIsNeedReCal;

		IconArrowState mIconArrowState;
		UIPicBoxPtr mIconArrow0;
		UIPicBoxPtr mIconArrow1;
		UIPicBoxPtr mIcon;
		UITextPtr mText;
	};

#include "PX2UIItem.inl"
	PX2_REGISTER_STREAM(UIItem);
	typedef Pointer0<UIItem> UIItemPtr;

}

#endif