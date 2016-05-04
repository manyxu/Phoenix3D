// PX2EU_PropertyGrid.hpp

#ifndef PX2EU_PROPERTYGRID_HPP
#define PX2EU_PROPERTYGRID_HPP

#include "PX2UIPropertyGrid.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_PropertyGrid : public UIPropertyGrid
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_PropertyGrid);

	public:
		EU_PropertyGrid();
		virtual ~EU_PropertyGrid();

		virtual void OnItemChanged(UIItem *item);

		virtual void DoExecute(Event *event);

	protected:
		void _RefreshSelect();
		void _ClearSelect();

		ObjectPtr mObject;
		NotFreeObjectPool<UIItem> mObjectPool;
	};

	PX2_REGISTER_STREAM(EU_PropertyGrid);
	typedef PointerRef<EU_PropertyGrid> EU_PropertyGridPtr;

}

#endif