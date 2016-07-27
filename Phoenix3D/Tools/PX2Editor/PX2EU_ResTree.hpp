// PX2EU_ResTree.hpp

#ifndef PX2EU_RESTREE_HPP
#define PX2EU_RESTREE_HPP

#include "PX2EditorPre.hpp"
#include "PX2UITree.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_ResTree : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResTree);

	public:
		enum ResTreeType
		{
			RTT_DIR,
			RTT_MAX_TYPE
		};

		EU_ResTree(ResTreeType rtt);
		virtual ~EU_ResTree();

		virtual UIItem *AddItem(UIItem *parentItem, const std::string &label,
			const std::string &name = "", Object *obj = 0);
		virtual void OnSelected(UIItem *item);

		enum RefreshType
		{
			RT_DIR,
			RT_DIR_FILES,
			RT_DIR_ALL,
			RT_DIR_ALL_NOCHILDREN,
			RT_MAX_TYPE
		};
		void RefreshItems(UIItem *parent, RefreshType type, bool isExpand);

		virtual void OnEvent(Event *event);

	protected:
		bool _IsAFile(const std::string &filename);

		UIItemPtr mDataItem;
		ResTreeType mResTreeType;
	};

	PX2_REGISTER_STREAM(EU_ResTree);
	typedef PointerRef<EU_ResTree> EU_ResTreePtr;

}

#endif