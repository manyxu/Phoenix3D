// PX2EU_ProjectTree.hpp

#ifndef PX2EU_PROJECTTREE_HPP
#define PX2EU_PROJECTTREE_HPP

#include "PX2UITree.hpp"
#include "PX2EU_ProjectItem.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ProjectTree : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectTree);

	public:
		EU_ProjectTree();
		virtual ~EU_ProjectTree();

		void SetShowType(ShowType st);
		ShowType GetShowType() const;

		virtual UIItem *AddItem(UIItem *parentItem, const std::string &label,
			const std::string &name = "", Object *obj = 0);

		virtual void DoExecute(Event *ent);
		virtual void OnSelected(UIItem *item);

	protected:
		void _RefreshProject();
		void _ClearProject();

		void _RefreshScene();
		void _ClearScene();
		void _RefreshOnMoveableScene(UIItem *parentItem, Movable *mov);

		void _RefreshUI();
		void _ClearUI();

		ShowType mShowType;

		UIItemPtr mItemProject;

		UIItemPtr mItemSetting;		
	
		UIItemPtr mItemSceneCanvas;

		UIItemPtr mItemUICanvas;
	};

	PX2_REGISTER_STREAM(EU_ProjectTree);
	typedef PointerRef<EU_ProjectTree> EU_ProjectTreePtr;

}

#endif