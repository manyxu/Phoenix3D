// PX2ASContext.hpp

#ifndef PX2ASCONTEXT_HPP
#define PX2ASCONTEXT_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"
#include "PX2ASClassType.hpp"

class asIObjectType;
class asIScriptContext;
class asIScriptEngine;
class asIScriptModule;
class asIScriptFunction;
struct asSMessageInfo;
class asIScriptObject;

namespace PX2
{

	class ScriptController;

	class PX2_FOUNDATION_ITEM ASContext : public ScriptContext
	{
	public:
		ASContext();
		virtual ~ASContext();

		// general
		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size);

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
		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

		// control
		virtual ScriptController *CreateScriptController(
			const std::string &filename, const std::string &className);
		void CallOnStart(asIScriptObject *object);
		void CallOnUpdate(asIScriptObject *object);

		// engine
		asIScriptEngine *GetASScriptEngine();

		bool IsHasCompileErrors;

	protected:
		void MessageCallback(const asSMessageInfo &msg);

		asIScriptObject *_CreateASScriptObject(const std::string &filename,
			const std::string &className, ScriptController *ctrl);
		asIScriptContext *_PrepareContextFromPool(asIScriptFunction *func);
		void _ReturnContextToPool(asIScriptContext *ctx);
		int _ContextExecute(asIScriptContext *ctx);


		ASClassType *_GetASClassType(const std::string &filename, 
			const std::string &className);
		void _BuildModule(const std::string &filename);

		asIScriptEngine *mASEngine;
		std::vector<asIScriptContext *> mContextsPool;
		std::vector<ASClassTypePtr> mASClassTypes;
		std::map<std::string, asIScriptModule*> mModules;
	};

}

#endif