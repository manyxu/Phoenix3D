// PX2EU_ResTree.hpp

#ifndef PX2EU_RESTREE_HPP
#define PX2EU_RESTREE_HPP

#include "PX2UITree.hpp"

namespace PX2
{

	class PX2_EXTENDS_ITEM EU_ResTree : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_ResTree);

	public:
		EU_ResTree();
		virtual ~EU_ResTree();
	};

	PX2_REGISTER_STREAM(EU_ResTree);
	typedef Pointer0<EU_ResTree> EU_ResTreePtr;

}

#endif