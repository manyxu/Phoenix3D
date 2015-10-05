// PX2UIItem.hpp

#ifndef PX2UIITEM_HPP
#define PX2UIITEM_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIButton.hpp"

namespace PX2
{

	class UIItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIItem);

	public:
		UIItem();
		virtual ~UIItem();

		UIItem *AddItem(const std::string &label);
		virtual void OnChildAdded(Movable *child);
		virtual void OnChildRemoved(Movable *child);
		int GetNumChildItem() const;

		void Expand(bool expand);
		bool IsExpand() const;
		int GetNumAllChildsExpand() const;

		void SetLabel(const std::string &label);
		const std::string &GetLabel() const;

		enum IconArrowState
		{
			IAS_NONE,
			IAS_ARROW0,
			IAS_ARROW1,
			IAS_MAX_STATE
		};
		void SetIconArrowState(IconArrowState state);
		IconArrowState GetIconArrowState() const;

		UIButton *GetButBack();
		UIPicBox *GetIconArrow0();
		UIPicBox *GetIconArrow1();
		UIPicBox *GetIcon();
		UIText *GetText();

		virtual void OnSizeChanged();

	public_internal:
		void _SetLevel(int level);
		int _GetLevel() const;

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _RecalNumChildExpand();
		void _TellParentChildrenRecal();
		void _Recal();

		bool mIsNeedRecal;

		bool mIsExpand;
		bool mIsNumAllChildExpandNeedRecal;
		int mNumAllChildExpand;

		IconArrowState mIconArrowState;
		UIButtonPtr mButBack;
		UIPicBoxPtr mIconArrow0;
		UIPicBoxPtr mIconArrow1;
		UIPicBoxPtr mIcon;
		UITextPtr mText;

		int mLevel;

		int mNumChildItem;
	};

#include "PX2UIItem.inl"
	PX2_REGISTER_STREAM(UIItem);
	typedef Pointer0<UIItem> UIItemPtr;

}

#endif