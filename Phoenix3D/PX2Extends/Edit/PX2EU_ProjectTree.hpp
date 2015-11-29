// PX2EU_ProjectTree.hpp

#ifndef PX2EU_PROJECTTREE_HPP
#define PX2EU_PROJECTTREE_HPP

#include "PX2UITree.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ProjectTree : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ProjectTree);

	public:
		enum ProjectTreeType
		{
			PTT_PROJECT,
			PTT_SCENE,
			PTT_UI,
			PTT_MAX_TYPE
		};
		EU_ProjectTree(ProjectTreeType ptt);
		virtual ~EU_ProjectTree();

		virtual void DoExecute(Event *ent);

	protected:
		void _RefreshProject();
		void _ClearProject();

		void _RefreshScene();
		void _ClearScene();
		void _RefreshOnMoveable(UIItem *parentItem, Movable *mov);

		void _RefreshUI();
		void _ClearUI();

		ProjectTreeType mProjectTreeType;
	};

	PX2_REGISTER_STREAM(EU_ProjectTree);
	typedef Pointer0<EU_ProjectTree> EU_ProjectTreePtr;

}

#endif