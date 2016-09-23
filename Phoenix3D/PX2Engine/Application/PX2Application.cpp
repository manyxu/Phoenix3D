// PX2Application.cpp

#include "PX2Application.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Application::Application() :
mPt_Data(0),

mDynLibMan(0),
mPluginMan(0),
mTimerMan(0),
mEventWorld(0),
mLanguageMan(0),
mRoot(0),
mIMEDisp(0),
mInputMan(0),
mResMan(0),
mScriptMan(0),
mFontMan(0),
mADMan(0),
mSelectionMan(0),
mURDoMan(0),
mFunObjectManager(0),
mAccoutManager(0),
mUIAuiManager(0),
mUISkinManager(0),
mLogicManager(0),
mBPManager(0),
mCreater(0),
mEngineEventHandler(0),

mIsInBackground(false),
mBeforeInBackgroundMusicEnable(true),
mBeforeInBackgroundSoundEnable(true),

mPlayType(PT_NONE),

mAppTime(0),
mLastAppTime(0),
mElapsedTime(0)
{
}
//----------------------------------------------------------------------------
Application::~Application()
{
}
//----------------------------------------------------------------------------
void Application::SetPt_Data(void *data)
{
	mPt_Data = data;
}
//----------------------------------------------------------------------------
void *Application::GetPt_Data()
{
	return mPt_Data;
}
//----------------------------------------------------------------------------
void Application::SetPt_Size(const Sizef &size)
{
	mPt_Size = size;
}
//----------------------------------------------------------------------------
const Sizef &Application::GetPt_Size() const
{
	return mPt_Size;
}
//----------------------------------------------------------------------------
inline float Application::GetElapsedTime()
{
	return (float)(mAppTime - mLastAppTime);
}
//----------------------------------------------------------------------------
void Application::Update()
{
	mAppTime = Time::GetTimeInSeconds();
	mElapsedTime = GetElapsedTime();
	mLastAppTime = mAppTime;

	// event
	if (mEventWorld)
		mEventWorld->Update((float)mElapsedTime);

	// resource
	PX2_RM.Update(mAppTime, mElapsedTime);

	// font
	PX2_FM.Update();

	// Plugin
	PX2_PLUGINMAN.Update();

	// graph
	PX2_GR.Update(mAppTime, mElapsedTime);

	if (mIsInBackground) return;

	PX2_GR.Draw();
}
//----------------------------------------------------------------------------