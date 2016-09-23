// PX2EU_ProjectItem.hpp

#ifndef PX2EU_PROJECTITEM_HPP
#define PX2EU_PROJECTITEM_HPP

#include "PX2EditorPre.hpp"
#include "PX2UIItem.hpp"

namespace PX2
{

	enum ShowType
	{
		ST_GENERAL,
		ST_CHILDREN,
		ST_CONTROLS,
		ST_MATERIAL,
		ST_DETAIL,
		ST_MAX_TYPE
	};

	class EU_ProjectTree;

	class PX2_EDITOR_ITEM EU_ProjectItem : public UIItem
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EU_ProjectItem);

	public:
		EU_ProjectItem();
		virtual ~EU_ProjectItem();

		void SetShowType(ShowType st);
		ShowType GetShowType() const;

		virtual void RemoveAllChildItems();

	public_internal:
		void SetEU_ProjectTree(EU_ProjectTree *tree);
		EU_ProjectTree *GetEU_ProjectTree();

	protected:
		EU_ProjectTree *mEU_ProjectTree;
		ShowType mShowType;
	};

	PX2_REGISTER_STREAM(EU_ProjectItem);
	typedef PointerRef<EU_ProjectItem> EU_ProjectItemPtr;

}

#endif