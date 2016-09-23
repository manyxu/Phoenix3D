// PX2EU_ProjectTree.hpp

#ifndef PX2EU_PROJECTTREE_HPP
#define PX2EU_PROJECTTREE_HPP

#include "PX2EditorPre.hpp"
#include "PX2UITree.hpp"
#include "PX2EU_ProjectItem.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_ProjectTree : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectTree);

	public:
		EU_ProjectTree();
		virtual ~EU_ProjectTree();

		void SetShowType(ShowType st);
		ShowType GetShowType() const;

		virtual UIItem *AddItem(UIItem *parentItem,
			const std::string &label, const std::string &name = "", 
			Object *obj = 0);

		virtual void OnEvent(Event *ent);
		virtual void OnSelected(UIItem *item, bool isDouble);
		virtual void OnUIPicked(const UIInputData &inputData);

	protected:
		void _RefreshProject();
		void _ClearProject();

		void _RefreshScene();
		void _ClearScene();
		void _RefreshOnMoveable(UIItem *parentItem, Movable *mov,
			int &expandLevel, bool isBP);

		void _RefreshUI();
		void _ClearUI();

		void _RefreshBP();
		void _ClearBP();

		ShowType mShowType;

		UIItemPtr mItemProject;
		UIItemPtr mItemScene;
		UIItemPtr mItemUI;
		UIItemPtr mItemBP;
	};

	PX2_REGISTER_STREAM(EU_ProjectTree);
	typedef PointerRef<EU_ProjectTree> EU_ProjectTreePtr;

}

#endif