// PX2EngineLoop_Tick.cpp

#include "PX2EngineLoop.hpp"
#include "PX2Project.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void EngineLoop::Tick()
{
	mAppTime = Time::GetTimeInSeconds();
	mElapsedTime = GetElapsedTime();
	mLastAppTime = mAppTime;

	if (mEventWorld)
		mEventWorld->Update((float)mElapsedTime);

	PX2_RM.Update(mAppTime, mElapsedTime);
	PX2_FM.Update();

	for (int i = 0; i < (int)mTickCallbacks.size(); i++)
	{
		(*mTickCallbacks[i])(mAppTime, mElapsedTime);
	}

	PX2_GR.Update(mAppTime, mElapsedTime);

	if (mIsInBackground) return;

	PX2_GR.Draw();
}
//----------------------------------------------------------------------------
void EngineLoop::AddTickCallback(TickCallback callback)
{
	if (IsHasTickCallback(callback))
		return;

	mTickCallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
bool EngineLoop::IsHasTickCallback(TickCallback callback)
{
	for (int i = 0; i < (int)mTickCallbacks.size(); i++)
	{
		if (callback == mTickCallbacks[i])
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineLoop::RemoveTickCallback(TickCallback callback)
{
	std::vector<TickCallback>::iterator it = mTickCallbacks.begin();
	for (; it != mTickCallbacks.end(); it++)
	{
		if (*it == callback)
		{
			mTickCallbacks.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void EngineLoop::ClearTickCallbacks()
{
	mTickCallbacks.clear();
}
//----------------------------------------------------------------------------