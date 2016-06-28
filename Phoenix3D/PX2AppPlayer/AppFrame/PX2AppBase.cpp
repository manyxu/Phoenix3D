// PX2AppBase.cpp

#include "PX2AppBase.hpp"
#include "PX2Project.hpp"
#include "PX2PluginManager.hpp"
#include "PX2Log.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Application.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool AppBase::msIsInitlized = false;
AppBase::AppInitlizeFun AppBase::msAppInitlizeFun = 0;
AppBase::AppTernamateFun AppBase::msAppTernamateFun = 0;
AppBase* AppBase::msApplication = 0;
AppBase::EntryPoint AppBase::msEntry = 0;
//----------------------------------------------------------------------------
AppBase::AppBase() :
mXPosition(0),
mYPosition(0),
mWidth(800),
mHeight(600),
mAllowResize(true)
{
	mWindowTitle = "App";
}
//----------------------------------------------------------------------------
AppBase::~AppBase ()
{
}
//----------------------------------------------------------------------------
bool AppBase::IsInitlized ()
{
	return msIsInitlized;
}
//----------------------------------------------------------------------------
bool AppBase::Initlize ()
{
	/*_*/
	return true;
}
//----------------------------------------------------------------------------
int AppBase::GetWidth() const
{
	return mWidth;
}
//----------------------------------------------------------------------------
int AppBase::GetHeight() const
{
	return mHeight;
}
//----------------------------------------------------------------------------
bool AppBase::OnIdle ()
{
	PX2_APP.Update();

	return true;
}
//----------------------------------------------------------------------------
bool AppBase::Terminate ()
{
	PX2_APP.Terminate();

	return true;
}
//----------------------------------------------------------------------------
void AppBase::SetTitle(const std::string &title)
{
	mWindowTitle = title;
}
//----------------------------------------------------------------------------
const std::string &AppBase::GetTitle() const
{
	return mWindowTitle;
}
//----------------------------------------------------------------------------
void AppBase::OnSize (int width, int height)
{
	if (0 == width || 0 == height) return;

	PX2_APP.SetScreenSize(Sizef((float)width, (float)height));
}
//----------------------------------------------------------------------------
void AppBase::WillEnterForeground(bool isFirstTime)
{
	PX2_APP.WillEnterForeground(isFirstTime);
}
//----------------------------------------------------------------------------
void AppBase::DidEnterBackground ()
{
	PX2_APP.DidEnterBackground();
}
//----------------------------------------------------------------------------
int AppBase::Main (int numArguments, char** arguments)
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