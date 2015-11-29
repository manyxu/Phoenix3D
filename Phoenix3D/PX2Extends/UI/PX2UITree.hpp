// PX2UITree.hpp

#ifndef PX2UITREE_HPP
#define PX2UITREE_HPP

#include "PX2UIItem.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UITree : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UITree);

	public:
		UITree();
		virtual ~UITree();

		void SetItemHeight(float height);
		float GetItemHeight() const;

		void SetIconArrowSpace(float space);
		float GetIconArrowSpace() const;

		UIItem *GetRootItem();
		UIItem *AddItem(UIItem *parentItem, const std::string &label);
		void RemoveAllItemsExceptRoot();
		void ShowRootItem(bool show);
		bool IsShowRootItem() const;

	protected:
		float mItemHeight;
		float mIconArrowSpace;
		UIItemPtr mRootItem;
		bool mIsShowRootItem;
	};

#include "PX2UITree.inl"
	PX2_REGISTER_STREAM(UITree);
	typedef Pointer0<UITree> UITreePtr;

}

#endif