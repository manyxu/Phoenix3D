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
		EU_ProjectTree();
		virtual ~EU_ProjectTree();
	};

	PX2_REGISTER_STREAM(EU_ProjectTree);
	typedef Pointer0<EU_ProjectTree> EU_ProjectTreePtr;

}

#endif