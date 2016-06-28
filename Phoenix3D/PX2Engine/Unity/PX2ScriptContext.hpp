// PX2ScriptContext.hpp

#ifndef PX2SCRIPTCONTEXT_HPP
#define PX2SCRIPTCONTEXT_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2ScriptController.hpp"

namespace PX2
{

	typedef void *(*ScriptGlobalFun) ();

	class PX2_ENGINE_ITEM ScriptContext : public RefObject
	{
	public:
		enum ContextType
		{
			CT_NONE,
			CT_LUA,
			CT_ANGELSCRIPT,
			CT_MAX_TYPE
		};

		ScriptContext(ContextType type = CT_NONE);
		virtual ~ScriptContext();

		ContextType GetType() const;

		// general
		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size,
			const std::string &name);

		// file function
		virtual bool CallFileFunction(const std::string &filename, 
			const std::string &funName);

		// type pointer function
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);
		virtual void SetUserFunction(const std::string &funName,
			const std::string &returnClassTypeName,
			ScriptGlobalFun globalFun);

		// object functions
		bool CallObjectFunction(const std::string &funName,
			Object *paramObj, const char *format, ...);
		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

		virtual ScriptController *CreateScriptController(
			const std::string &filename, const std::string &className);

		virtual void RegistOperators();

	protected:
		ContextType mType;
	};
	typedef PointerRef<ScriptContext> ScriptContextPtr;

}

#endif