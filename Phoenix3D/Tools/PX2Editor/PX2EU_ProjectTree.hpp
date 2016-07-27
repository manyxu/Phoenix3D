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

		virtual UIItem *AddItem(UIItem *parentItem, const std::string &label,
			const std::string &name = "", Object *obj = 0);

		virtual void OnEvent(Event *ent);
		virtual void OnSelected(UIItem *item);

	protected:
		void _RefreshScene();
		void _ClearScene();
		void _RefreshOnMoveableScene(UIItem *parentItem, Movable *mov);

		ShowType mShowType;
	};

	PX2_REGISTER_STREAM(EU_ProjectTree);
	typedef PointerRef<EU_ProjectTree> EU_ProjectTreePtr;

}

#endif