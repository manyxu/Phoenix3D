// PX2EU_PropertyGridEdit.hpp

#ifndef PX2EU_PROPERTYGRIDEDIT_HPP
#define PX2EU_PROPERTYGRIDEDIT_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIPropertyGrid.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_PropertyGridEdit : public UIPropertyGrid
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_PropertyGridEdit);

	public:
		EU_PropertyGridEdit();
		virtual ~EU_PropertyGridEdit();

		virtual void OnItemChanged(UIItem *item);

		virtual void OnEvent(Event *event);

	protected:
		void _RefreshSelect();
		void _ClearSelect();

		ObjectPtr mObject;
	};

	PX2_REGISTER_STREAM(EU_PropertyGridEdit);
	typedef PointerRef<EU_PropertyGridEdit> EU_PropertyGridEditPtr;

}

#endif