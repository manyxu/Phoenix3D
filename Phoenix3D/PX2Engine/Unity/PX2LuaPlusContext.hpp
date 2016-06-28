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

		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size, 
			const std::string &name);

		virtual bool CallFileFunction(const std::string &filename,
			const std::string &funName);

		// type pointer function
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);

	protected:
		void OnError(int errorNum);
		void ClearStack(void);

		LuaPlus::LuaState* mLuaPlusState;
		std::string mLastError;
	};

}

#endif