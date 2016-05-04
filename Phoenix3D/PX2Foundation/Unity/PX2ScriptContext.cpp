// PX2ScriptContext.cpp

#include "PX2ScriptContext.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScriptContext::ScriptContext(ContextType type) :
mType(type)
{
}
//----------------------------------------------------------------------------
ScriptContext::~ScriptContext()
{

}
//----------------------------------------------------------------------------
ScriptContext::ContextType ScriptContext::GetType() const
{
	return mType;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallString(const std::string &str)
{
	PX2_UNUSED(str);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFile(const std::string &filename)
{
	PX2_UNUSED(filename);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallBuffer(const char *buffer, unsigned long size)
{
	PX2_UNUSED(buffer);
	PX2_UNUSED(size);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFileFunction(const std::string &filename, 
	const std::string &funName)
{
	PX2_UNUSED(filename);
	PX2_UNUSED(funName);

	return true;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallObjectFunction(const char *objectName,
	const char *funName, const char *format, ...)
{
	PX2_UNUSED(objectName);
	PX2_UNUSED(funName);
	PX2_UNUSED(format);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallObjectFuntionValist(const char *objectName,
	const char *funName, const char *format, va_list valist)
{
	PX2_UNUSED(objectName);
	PX2_UNUSED(funName);
	PX2_UNUSED(format);
	PX2_UNUSED(valist);

	return false;
}
//----------------------------------------------------------------------------
void ScriptContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	PX2_UNUSED(luaName);
	PX2_UNUSED(className);
	PX2_UNUSED(ptr);
}
//----------------------------------------------------------------------------
void ScriptContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName, ScriptGlobalFun globalFun)
{
	PX2_UNUSED(returnClassTypeName);
	PX2_UNUSED(funName);
	PX2_UNUSED(globalFun);
}
//----------------------------------------------------------------------------
ScriptController *ScriptContext::CreateScriptController(
	const std::string &filename, const std::string &className)
{
	PX2_UNUSED(filename);
	PX2_UNUSED(className);

	return 0;
}
//----------------------------------------------------------------------------