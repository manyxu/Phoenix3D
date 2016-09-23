// PX2EU_CF_BluePrint.hpp

#ifndef PX2EU_CF_BLUEPRINT_HPP
#define PX2EU_CF_BLUEPRINT_HPP

#include "PX2EditorPre.hpp"
#include "PX2EU_ProjectTree.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EU_CF_BluePrint : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(EU_CF_BluePrint);

	public:
		EU_CF_BluePrint();
		virtual ~EU_CF_BluePrint();

	protected:
		UITreePtr mTree;
	};

	PX2_REGISTER_STREAM(EU_CF_BluePrint);
	typedef PointerRef<EU_CF_BluePrint> EU_CF_BluePrintPtr;

}

#endif