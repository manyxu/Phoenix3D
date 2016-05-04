// PX2LuaContext.hpp

#ifndef PX2LUACONTEXT_HPP
#define PX2LUACONTEXT_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"

struct lua_State;

namespace PX2
{

	class PX2_FOUNDATION_ITEM LuaStackBackup
	{
	public:
		LuaStackBackup (lua_State *luaState);
		~LuaStackBackup ();

	private:
		lua_State *mState;
		int	mTop;
	};

	class PX2_FOUNDATION_ITEM LuaContext : public ScriptContext
	{
	public:
		LuaContext();
		virtual ~LuaContext();

		void SetLuaState (lua_State *state);
		lua_State *GetLuaState ();

		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size);

		// call start functions
		virtual bool CallFileFunction(const std::string &filename,
			const std::string &funName);

		virtual bool CallObjectFunction(const char *objectName,
			const char *funName,
			const char *format = "", ...);
		// 调用者需要保证objectName是全局唯一的
		virtual bool CallObjectFuntionValist(const char *objectName,
			const char *funName, const char *format, va_list valist);
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);


	protected:
		lua_State *mState;
	};

#include "PX2LuaContext.inl"

}

#endif