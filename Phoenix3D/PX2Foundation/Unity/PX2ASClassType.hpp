// PX2ASClassType.hpp

#ifndef PX2ASCLASSTYPE_HPP
#define PX2ASCLASSTYPE_HPP

#include "PX2UnityPre.hpp"
#include "PX2SmartPointer.hpp"

class asIObjectType;
class asIScriptContext;
class asIScriptEngine;
class asIScriptModule;
struct asSMessageInfo;
class asIScriptFunction;

namespace PX2
{

	class PX2_FOUNDATION_ITEM ASClassType
	{
	public:
		ASClassType();
		virtual ~ASClassType();

		std::string ClassName;
		asIObjectType *ClassType;
		asIScriptFunction *FactoryFunc;
		asIScriptFunction *OnStartFunc;
		asIScriptFunction *OnUpdateFunc;
	};
	typedef Pointer0<ASClassType> ASClassTypePtr;

}

#endif