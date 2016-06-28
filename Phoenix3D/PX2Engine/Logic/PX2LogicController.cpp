// PX2LogicController.cpp

#include "PX2LogicController.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ScriptController, LogicController);
PX2_IMPLEMENT_STREAM(LogicController);
PX2_IMPLEMENT_FACTORY(LogicController);
PX2_IMPLEMENT_DEFAULT_NAMES(ScriptController, LogicController);

//---------------------------------------------------------------------------
LogicController::LogicController()
{
	mModule = new0 LogicModule();
}
//---------------------------------------------------------------------------
LogicController::~LogicController()
{
}
//---------------------------------------------------------------------------
void LogicController::FixUpdate()
{
	if (mModule)
	{
		mModule->Update();
	}
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LogicController::LogicController(LoadConstructor value) :
ScriptController(value)
{
}
//----------------------------------------------------------------------------
void LogicController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ScriptController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LogicController, source);
}
//----------------------------------------------------------------------------
void LogicController::Link(InStream& source)
{
	ScriptController::Link(source);

	source.ResolveLink(mObject);
}
//----------------------------------------------------------------------------
void LogicController::PostLink()
{
	ScriptController::PostLink();
}
//----------------------------------------------------------------------------
bool LogicController::Register(OutStream& target) const
{
	if (ScriptController::Register(target))
	{
		target.Register(mObject);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LogicController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ScriptController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LogicController, target);
}
//----------------------------------------------------------------------------
int LogicController::GetStreamingSize(Stream &stream) const
{
	int size = ScriptController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------