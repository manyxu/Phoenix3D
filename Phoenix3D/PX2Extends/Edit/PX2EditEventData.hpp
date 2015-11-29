// PX2EditEventData.hpp

#ifndef PX2EDITEVENTDATA_HPP
#define PX2EDITEVENTDATA_HPP

#include "PX2EditPre.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EED_CreateWindow
	{
	public:
		EED_CreateWindow();
		~EED_CreateWindow();

		bool IsMain;
		std::string Name;
		int ID;
		Float2 Pos;
	};

	class PX2_EXTENDS_ITEM EED_AddMenu
	{
	public:
		EED_AddMenu();
		~EED_AddMenu();

		std::string Where;

		enum ItemType
		{
			IT_MAIN_MENU,
			IT_SUB_MENU,
			IT_ITEM,
			IT_ITEMSPARATER,
			IT_MAX_TYPE
		};
		ItemType TheItemType;

		std::string ParentName;
		std::string Name;
		std::string Title;
		std::string Script;
		std::string Tag;
	};

}

#endif
