// PX2ASClassType.hpp

#ifndef PX2ASCLASSTYPE_HPP
#define PX2ASCLASSTYPE_HPP

#include "PX2UnityPre.hpp"
#include "PX2SmartPointer.hpp"

class asITypeInfo;
class asIScriptContext;
class asIScriptEngine;
class asIScriptModule;
struct asSMessageInfo;
class asIScriptFunction;

namespace PX2
{

	class PX2_ENGINE_ITEM ASClassType
	{
	public:
		ASClassType();
		virtual ~ASClassType();

		std::string ClassName;
		asITypeInfo *ClassType;
		asIScriptFunction *FactoryFunc;
		asIScriptFunction *OnAttachedFunc;
		asIScriptFunction *OnDetachFunc;
		asIScriptFunction *OnInitUpdateFunc;
		asIScriptFunction *OnUpdateFunc;
		asIScriptFunction *OnFixUpdateFunc;

		asIScriptFunction *OnResetPlayFunc;
		asIScriptFunction *OnResetFunc;
		asIScriptFunction *OnPlayFunc;
		asIScriptFunction *OnStopFunc;
		asIScriptFunction *OnPlayUpdateFunc;
	};
	typedef Pointer0<ASClassType> ASClassTypePtr;

}

#endif