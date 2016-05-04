// PX2ApplicationBase.cpp

#include "PX2ApplicationBase.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2Project.hpp"
#include "PX2PluginManager.hpp"
#include "PX2Log.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool ApplicationBase::msIsInitlized = false;
ApplicationBase::AppInitlizeFun ApplicationBase::msAppInitlizeFun = 0;
ApplicationBase::AppTernamateFun ApplicationBase::msAppTernamateFun = 0;
ApplicationBase* ApplicationBase::msApplication = 0;
ApplicationBase::EntryPoint ApplicationBase::msEntry = 0;
//----------------------------------------------------------------------------
ApplicationBase::ApplicationBase() :
mXPosition(0),
mYPosition(0),
mWidth(800),
mHeight(600),
mAllowResize(true)
{
	mWindowTitle = "Application";
}
//----------------------------------------------------------------------------
ApplicationBase::~ApplicationBase ()
{
}
//----------------------------------------------------------------------------
bool ApplicationBase::IsInitlized ()
{
	return msIsInitlized;
}
//----------------------------------------------------------------------------
bool ApplicationBase::Initlize ()
{
	/*_*/
	return true;
}
//----------------------------------------------------------------------------
int ApplicationBase::GetWidth() const
{
	return mWidth;
}
//----------------------------------------------------------------------------
int ApplicationBase::GetHeight() const
{
	return mHeight;
}
//----------------------------------------------------------------------------
bool ApplicationBase::OnIdle ()
{
	PX2_ENGINELOOP.Tick();

	return true;
}
//----------------------------------------------------------------------------
bool ApplicationBase::Terminate ()
{
	PX2_ENGINELOOP.Terminate();

	return true;
}
//----------------------------------------------------------------------------
void ApplicationBase::SetTitle(const std::string &title)
{
	mWindowTitle = title;
}
//----------------------------------------------------------------------------
const std::string &ApplicationBase::GetTitle() const
{
	return mWindowTitle;
}
//----------------------------------------------------------------------------
void ApplicationBase::OnSize (int width, int height)
{
	if (0 == width || 0 == height) return;

	PX2_ENGINELOOP.SetScreenSize(Sizef((float)width, (float)height));
}
//----------------------------------------------------------------------------
void ApplicationBase::WillEnterForeground(bool isFirstTime)
{
	PX2_ENGINELOOP.WillEnterForeground(isFirstTime);
}
//----------------------------------------------------------------------------
void ApplicationBase::DidEnterBackground ()
{
	PX2_ENGINELOOP.DidEnterBackground();
}
//----------------------------------------------------------------------------
int ApplicationBase::Main (int numArguments, char** arguments)
{
	PX2_UNUSED(numArguments);
	PX2_UNUSED(arguments);

	if (numArguments > 1)
	{
		mCmdProjectName = std::string(arguments[1]);
	}

	return 1;
}
//----------------------------------------------------------------------------