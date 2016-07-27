// PX2EU_ResList.hpp

#ifndef PX2EU_RESLIST_HPP
#define PX2EU_RESLIST_HPP

#include "PX2UIList.hpp"
#include "PX2EditorPre.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_ResList : public UIList
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResList);

	public:
		EU_ResList();
		virtual ~EU_ResList();

		virtual void OnSelected(UIItem *item);

		void OnEvent(Event *event);

	protected:
		void RefreshItems(const std::string &path);

		std::string mSelectPath;
	};

	PX2_REGISTER_STREAM(EU_ResList);
	typedef PointerRef<EU_ResList> EU_ResListPtr;

}

#endif