// PX2EditObject.hpp

#ifndef PX2EDITOBJECT_HPP
#define PX2EDITOBJECT_HPP

#include "PX2EditorPre.hpp"
#include "PX2Object.hpp"
#include "PX2SmartPointerRef.hpp"

namespace PX2
{

	class PX2_EDITOR_ITEM EditObject : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EditObject);
		PX2_DECLARE_PROPERTY;
	
	public:
		EditObject();
		virtual ~EditObject();
	};

	PX2_REGISTER_STREAM(EditObject);
	typedef PointerRef<EditObject> EditObjectPtr;

}

#endif