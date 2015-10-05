// PX2UIList.hpp

#ifndef PX2UILIST_HPP
#define PX2UILIST_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIItem.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM UIList : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIList);

	public:
		UIList();
		virtual ~UIList();

		void SetItemHeight(float height);
		float GetItemHeight() const;

		UIItem *AddItem(const std::string &text);
		virtual void OnChildAdded(Movable *child);
		virtual void OnChildRemoved(Movable *child);

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _Recal();

		bool mIsNeedRecal;
		float mItemHeight;
		std::vector<UIItemPtr> mItems;
	};

#include "PX2UIList.inl"
	PX2_REGISTER_STREAM(UIList);
	typedef Pointer0<UIList> UIListPtr;

}

#endif