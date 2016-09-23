// PX2LuaPlusContext.hpp

#ifndef PX2LUAPLUSCONTEXT_HPP
#define PX2LUAPLUSCONTEXT_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"

namespace LuaPlus
{
	class LuaState;
}

namespace PX2
{

	class PX2_ENGINE_ITEM LuaPlusContext : public ScriptContext
	{
	public:
		LuaPlusContext();
		virtual ~LuaPlusContext();

		LuaPlus::LuaState *GetLuaPlusState();
		void *GetLuaState();

		bool CallString1(const char *str);
		bool CallFile1(const char *filename);

		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size, 
			const std::string &name);

		virtual bool CallFileFunction(const std::string &filename,
			const std::string &funName);

		// type pointer function
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);
		virtual void SetUserFunction(const std::string &funName,
			const std::string &returnClassTypeName,
			ScriptGlobalFun globalFun);

		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

		//virtual ScriptController *CreateScriptController(
		//	const std::string &filename, const std::string &className);

	protected:
		void OnError(int errorNum);
		void ClearStack(void);

		static LuaPlusContext *mSingletonLuaPlusContext;

		LuaPlus::LuaState* mLuaPlusState;
		std::string mLastError;
	};

}

#endif