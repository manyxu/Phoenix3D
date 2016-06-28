// AppPlayerApp.cpp

#include "AppPlayerApp.hpp"
#include "PX2Renderer.hpp"
#include "PX2Application.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_APPLICATION(AppPlayerApp)
//----------------------------------------------------------------------------
AppPlayerApp::AppPlayerApp ()
{
	mWindowTitle = "AppPlayer";
	mWindowTitle += Renderer::GetRenderTag();

#if defined(_WIN64) || defined(WIN64)
	mWindowTitle += "64";
#endif

#ifdef _DEBUG
	mWindowTitle += "D";
#endif
}
//----------------------------------------------------------------------------
AppPlayerApp::~AppPlayerApp ()
{
	// 所有内存释放,必须在析构函数之前释放
}
//----------------------------------------------------------------------------
bool AppPlayerApp::Initlize()
{
	if (App::Initlize())
	{
		std::string projectName = PX2_APP.GetProjectName();

		if (!mCmdProjectName.empty())
			projectName = mCmdProjectName;

		if (!projectName.empty())
		{
			std::string projectPath = "Data/" + projectName + "/" + projectName
				+ ".px2proj";

			// Load Project
			if (PX2_APP.LoadProject(projectPath))
			{
				PX2_APP.Play(Application::PT_PLAY);
			}

			Project *proj = Project::GetSingletonPtr();
			if (proj) SetTitle(projectName);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
int main(int numArguments, char* arguments[])
{
	AppBase::msAppInitlizeFun();

	int exitCode = AppBase::msEntry(numArguments, arguments);

	AppBase::msAppTernamateFun();

	return exitCode;
}
//----------------------------------------------------------------------------