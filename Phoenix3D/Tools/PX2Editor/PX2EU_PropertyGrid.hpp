// PX2EU_PropertyGrid.hpp

#ifndef PX2EU_PROPERTYGRID_HPP
#define PX2EU_PROPERTYGRID_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIPropertyGrid.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_PropertyGrid : public UIPropertyGrid
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_PropertyGrid);

	public:
		EU_PropertyGrid();
		virtual ~EU_PropertyGrid();

		virtual void OnItemChanged(UIItem *item);

		virtual void OnEvent(Event *event);

	protected:
		void _RefreshSelect();
		void _ClearSelect();

		ObjectPtr mObject;
	};

	PX2_REGISTER_STREAM(EU_PropertyGrid);
	typedef PointerRef<EU_PropertyGrid> EU_PropertyGridPtr;

}

#endif