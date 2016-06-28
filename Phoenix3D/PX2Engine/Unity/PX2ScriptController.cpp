// PX2ScriptController.cpp

#include "PX2ScriptController.hpp"
#include "PX2Controlledable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, ScriptController);
PX2_IMPLEMENT_STREAM(ScriptController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, ScriptController);
PX2_IMPLEMENT_FACTORY(ScriptController);

//---------------------------------------------------------------------------
ScriptController::ScriptController():
mIsInitUpdated(false),
mNumFixUpdatePerSeconds(30),
mFiexUpdateTiming(0)
{
	mOneFrameSeconds = 1.0f / (float)mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------
ScriptController::~ScriptController()
{
}
//---------------------------------------------------------------------------
void ScriptController::SetControlledable(Controlledable* object)
{
	if (!object)
	{
		ScriptOnDetach();
	}

	Controller::SetControlledable(object);

	if (object)
	{
		ScriptOnAttached();
	}
}
//---------------------------------------------------------------------------
bool ScriptController::Update(double applicationTime, double elapsedTime)
{
	if (!mIsInitUpdated)
	{
		ScriptOnInitUpdate();

		mIsInitUpdated = true;
	}

	ScriptOnUpdate();

	mFiexUpdateTiming += (float)elapsedTime;
	int numGenFrams = (int)(mFiexUpdateTiming*mNumFixUpdatePerSeconds);
	if (numGenFrams > 0)
	{
		for (int i = 0; i< numGenFrams; i++)
		{
			FixUpdate();
		}
		mFiexUpdateTiming -= (float)(numGenFrams* mOneFrameSeconds);
	}

	return Controller::Update(applicationTime, elapsedTime);
}
//---------------------------------------------------------------------------
void ScriptController::FixUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ResetPlay()
{
	ScriptOnResetPlay();

	Controller::ResetPlay();
}
//---------------------------------------------------------------------------
void ScriptController::Reset()
{
	ScriptOnReset();

	Controller::Reset();
}
//---------------------------------------------------------------------------
void ScriptController::Play()
{
	ScriptOnPlay();

	Controller::Play();
}
//---------------------------------------------------------------------------
void ScriptController::Stop()
{
	ScriptOnStop();

	Controller::Stop();
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnAttached()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnDetach()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnInitUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnFixUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnResetPlay()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnReset()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnPlay()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnStop()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnPlayUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::SetNumFixUpdatePerSeconds(int num)
{
	mNumFixUpdatePerSeconds = num;
	mOneFrameSeconds = 1.0f / mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------
int ScriptController::GetNumFixUpdatePerSeconds() const
{
	return mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
ScriptController::ScriptController(LoadConstructor value) :
Controller(value),
mIsInitUpdated(false),
mNumFixUpdatePerSeconds(30),
mFiexUpdateTiming(0)
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