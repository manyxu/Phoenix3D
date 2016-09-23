// PX2EU_PropertyGrid.hpp

#ifndef PX2EU_PROPERTYGRIDRES_HPP
#define PX2EU_PROPERTYGRIDRES_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIPropertyGrid.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_PropertyGridRes : public UIPropertyGrid
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_PropertyGridRes);

	public:
		EU_PropertyGridRes();
		virtual ~EU_PropertyGridRes();

		virtual void OnItemChanged(UIItem *item);

		virtual void OnEvent(Event *event);
	};

	PX2_REGISTER_STREAM(EU_PropertyGridRes);
	typedef PointerRef<EU_PropertyGridRes> EU_PropertyGridResPtr;

}

#endif