// PX2LogicModule.cpp

#include "PX2LogicModule.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, LogicModule);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, LogicModule);

//----------------------------------------------------------------------------
LogicModule::LogicModule() :
TheType(T_NONE),
FrameCount(0),
UpdateFrameCount(0)
{
}
//----------------------------------------------------------------------------
LogicModule::~LogicModule()
{

}
//----------------------------------------------------------------------------
LogicModule::State LogicModule::Update()
{
	if (S_OVER == TheState)
		return TheState;

	if (T_FUNCTION == TheType)
	{

	}

	LogicModule *updateM = NextModule;
	if (updateM && S_OVER == updateM->Update())
	{
		updateM = updateM->NextModule;
	}

	UpdateFrameCount++;
	if (UpdateFrameCount == FrameCount)
	{
		TheState = S_OVER;
	}

	return TheState;
}
//----------------------------------------------------------------------------