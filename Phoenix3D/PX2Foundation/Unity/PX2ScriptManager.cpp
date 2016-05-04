// PX2ScriptManager.cpp

#include "PX2ScriptManager.hpp"
#include "PX2LuaContext.hpp"
#include "PX2ASContext.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScriptManager::ScriptManager()
{
	mContextLua = new0 LuaContext();
	mContextAS= new0 ASContext();
}
//----------------------------------------------------------------------------
ScriptManager::~ScriptManager()
{
}
//----------------------------------------------------------------------------
void ScriptManager::Ternimate(ScriptContext::ContextType type)
{
	if (type == ScriptContext::CT_LUA)
		mContextLua = 0;

	if (type == ScriptContext::CT_ANGELSCRIPT)
		mContextAS = 0;
}
//----------------------------------------------------------------------------
void ScriptManager::TernimateAll()
{
	mContextLua = 0;
	mContextAS = 0;
}
//----------------------------------------------------------------------------
ScriptController *ScriptManager::CreateScriptController(
	const std::string &filename)
{
	std::string outPath;
	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExt);

	if ("as" == outExt)
	{
		return mContextAS->CreateScriptController(filename, outBaseName);
	}
	else if ("lua" == outExt)
	{
		return mContextLua->CreateScriptController(filename, outBaseName);
	}

	return 0;
}
//----------------------------------------------------------------------------
ScriptContext *ScriptManager::GetContext(ScriptContext::ContextType type)
{
	if (ScriptContext::CT_LUA == type)
	{
		return mContextLua;
	}
	else if (ScriptContext::CT_ANGELSCRIPT == type)
	{
		return mContextAS;
	}

	return 0;
}
//----------------------------------------------------------------------------