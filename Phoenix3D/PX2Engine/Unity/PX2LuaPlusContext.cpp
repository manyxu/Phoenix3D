// PX2LuaPlusContext.cpp

#include "PX2LuaPlusContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
#include "PX2LuaStackBackup.hpp"
#include "PX2LuaScriptController.hpp"
#include "LuaPlus.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;
using namespace LuaPlus;

//----------------------------------------------------------------------------
LuaPlusContext *LuaPlusContext::mSingletonLuaPlusContext = 0;
//----------------------------------------------------------------------------
LuaPlusContext::LuaPlusContext() :
ScriptContext(ScriptContext::CT_LUA),
mLuaPlusState(0)
{
	mSingletonLuaPlusContext = this;

	mLuaPlusState = LuaPlus::LuaState::Create(true);

	mLuaPlusState->OpenLibs();

	mLuaPlusState->GetGlobals().RegisterDirect("CallFile", (*this), 
		&LuaPlusContext::CallFile1);
	mLuaPlusState->GetGlobals().RegisterDirect("CallString", (*this), 
		&LuaPlusContext::CallString1);
}
//----------------------------------------------------------------------------
LuaPlusContext::~LuaPlusContext()
{
	LuaPlus::LuaState::Destroy(mLuaPlusState);
}
//----------------------------------------------------------------------------
LuaPlus::LuaState *LuaPlusContext::GetLuaPlusState()
{
	return mLuaPlusState;
}
//----------------------------------------------------------------------------
void *LuaPlusContext::GetLuaState()
{
	return mLuaPlusState->GetCState();
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString1(const char *str)
{
	return CallString(str);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile1(const char *filename)
{
	return CallFile(filename);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallString(const std::string &str)
{
	int result = 0;

	if (str.length() <= 1 || str[0] != '=')
	{
		result = mLuaPlusState->DoString(str.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}
	// If the string starts with '=', wrap the statement in the print() function.
	else
	{
		std::string buffer("print(");
		buffer += (str.c_str() + 1);
		buffer += ")";
		result = mLuaPlusState->DoString(buffer.c_str());
		if (result != 0)
		{
			OnError(result);
			return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFile(const std::string &filename)
{
	bool ret = false;

	int bufferSize = 0;
	char *buffer = 0;
	if (ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize,
		buffer))
	{
		ret = CallBuffer(buffer, (unsigned long)bufferSize, filename);
	}

	return ret;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	int result = mLuaPlusState->DoBuffer(buffer, size, name.c_str());
	if (result != 0)
	{
		OnError(result);
	}

	return true;
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	if (!CallFile(filename))
		return false;

	LuaObject funcObj = mLuaPlusState->GetGlobal(funName.c_str());
	if (!funcObj.IsNil())
	{
		LuaCall call = funcObj;
		
		try
		{
			call << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	LuaStackBackup stackbackup((lua_State*)mLuaPlusState);
	tolua_pushusertype((lua_State*)mLuaPlusState, ptr, className.c_str());
	lua_setglobal((lua_State*)mLuaPlusState, luaName.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName,
	ScriptGlobalFun globalFun)
{
	PX2_UNUSED(returnClassTypeName);

	mLuaPlusState->GetGlobals().RegisterDirect(funName.c_str(), globalFun);
}
//----------------------------------------------------------------------------
bool LuaPlusContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	lua_State *state = (lua_State*)mLuaPlusState;

	lua_getglobal(state, "this");
	lua_getglobal(state, paramObj->GetName().c_str());
	lua_setglobal(state, "this");

	const char *pfmt = format.c_str();
	int count = 0;
	static const char *args[] = { "arg0", "arg1", "arg2", "arg3" };
	while (pfmt[count])
	{
		if (*pfmt == 'i')
		{
			int value = va_arg(valist, int);
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 'f')
		{
			float value = (float)(va_arg(valist, double));
			lua_pushnumber(state, value);
		}
		else if (*pfmt == 's')
		{
			char *str = va_arg(valist, char *);
			lua_pushstring(state, str);
		}
		else
		{
			assertion(false, "");
		}
		lua_setglobal(state, args[count++]);
	}

	{
		CallString(funName);
	}

	lua_setglobal(state, "this");

	return true;
}
////----------------------------------------------------------------------------
//ScriptController *LuaPlusContext::CreateScriptController(
//	const std::string &filename, const std::string &className)
//{
//	LuaScriptController *luaScriptCtrl = new0 LuaScriptController(filename,
//		className);
//
//	return luaScriptCtrl;
//}
//----------------------------------------------------------------------------
void LuaPlusContext::OnError(int errorNum)
{
	PX2_UNUSED(errorNum);

	LuaPlus::LuaStackObject stackObj(mLuaPlusState, -1);
	const char* errStr = stackObj.GetString();
	if (errStr)
	{
		mLastError = errStr;
		ClearStack();
	}
	else
	{
		mLastError = "Unknown Lua parse error";
	}

	PX2_LOG_ERROR("LuaError:%s", mLastError.c_str());
}
//----------------------------------------------------------------------------
void LuaPlusContext::ClearStack(void)
{
	mLuaPlusState->SetTop(0);
}
//----------------------------------------------------------------------------