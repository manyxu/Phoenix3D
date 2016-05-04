// PX2ScriptController.cpp

#include "PX2ScriptController.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, ScriptController);
PX2_IMPLEMENT_STREAM(ScriptController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, ScriptController);
PX2_IMPLEMENT_FACTORY(ScriptController);

//---------------------------------------------------------------------------
ScriptController::ScriptController()
{
}
//---------------------------------------------------------------------------
ScriptController::~ScriptController()
{
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
ScriptController::ScriptController(LoadConstructor value) :
Controller(value)
{
}
//----------------------------------------------------------------------------
void ScriptController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ScriptController, source);
}
//----------------------------------------------------------------------------
void ScriptController::Link(InStream& source)
{
	Controller::Link(source);

	source.ResolveLink(mObject);
}
//----------------------------------------------------------------------------
void ScriptController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool ScriptController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		target.Register(mObject);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ScriptController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ScriptController, target);
}
//----------------------------------------------------------------------------
int ScriptController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------