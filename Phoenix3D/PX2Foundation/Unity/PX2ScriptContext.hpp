// PX2ScriptContext.hpp

#ifndef PX2SCRIPTCONTEXT_HPP
#define PX2SCRIPTCONTEXT_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2ScriptController.hpp"

namespace PX2
{

	typedef void *(*ScriptGlobalFun) ();

	class PX2_FOUNDATION_ITEM ScriptContext : public RefObject
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

		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size);

		// call start functions
		virtual bool CallFileFunction(const std::string &filename, 
			const std::string &funName);

		// object functions
		virtual bool CallObjectFunction(const char *objectName,
			const char *funName,
			const char *format = "", ...); 
		// 调用者需要保证objectName是全局唯一的
		virtual bool CallObjectFuntionValist(const char *objectName,
			const char *funName, const char *format, va_list valist);

		// general functions
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);	
		virtual void SetUserFunction(const std::string &funName, 
			const std::string &returnClassTypeName,
			ScriptGlobalFun globalFun);

		virtual ScriptController *CreateScriptController(
			const std::string &filename, const std::string &className);

	protected:
		ContextType mType;
	};
	typedef Pointer0<ScriptContext> ScriptContextPtr;

}

#endif