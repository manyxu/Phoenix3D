// PX2LuaPlusContext.cpp

#include "PX2LuaPlusContext.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Assert.hpp"
#include "PX2Log.hpp"
#include "PX2LuaStackBackup.hpp"
#include "LuaPlus.h"
using namespace PX2;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "tolua++.h"
using namespace PX2;

//----------------------------------------------------------------------------
LuaPlusContext::LuaPlusContext() :
ScriptContext(ScriptContext::CT_LUA),
mLuaPlusState(0)
{
	mLuaPlusState = LuaPlus::LuaState::Create(true);
	//mLuaPlusState->GetGlobals().RegisterDirect("CallFile", (*this), &LuaPlusContext::CallFile);
	//mLuaPlusState->GetGlobals().RegisterDirect("CallString", (*this), &LuaPlusContext::CallString);
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
	PX2_UNUSED(funName);
	return CallFile(filename);
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
void LuaPlusContext::OnError(int errorNum)
{
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