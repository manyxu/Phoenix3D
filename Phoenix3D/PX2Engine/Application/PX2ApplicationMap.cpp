// PX2ApplicationMap.cpp

#include "PX2Application.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2PluginManager.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Application::NewProject(const std::string &pathname,
	const std::string &projName, int so, int width, int height)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	CloseProject();

	TheProject = new0 Project();
	TheProject->SetName(projName);
	TheProject->SetScreenOrientation((Project::ScreenOrientation)so);
	TheProject->SetSize((float)width, (float)height);
	TheProject->Save(pathname);
	mProjectFilePath = pathname;

	Event *ent = PX2_CREATEEVENTEX(ProjectES, NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	Event *entUI = PX2_CREATEEVENTEX(ProjectES, NewUI);
	PX2_EW.BroadcastingLocalEvent(entUI);
}
//----------------------------------------------------------------------------
bool Application::LoadProject(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	CloseProject();

	TheProject = new0 Project();
	if (TheProject->Load(pathname))
	{
		std::string debugTag = "";
#if defined (_DEBUG) 
		debugTag = "D";
#endif
		std::string projName = PX2_PROJ.GetName();
		std::string folder = "Projects/" + projName + "/";
		std::string projDllPath = folder + projName + debugTag + ".dll";

		if (PX2_RM.IsFloderExist("", folder))
		{
			PX2_PLUGINMAN.Load(projDllPath);
		}

		Event *ent = PX2_CREATEEVENTEX(ProjectES, LoadedProject);
		PX2_EW.BroadcastingLocalEvent(ent);

		std::string sceneFilename = TheProject->GetSceneFilename();
		if (!sceneFilename.empty())
		{
			LoadScene(sceneFilename);
		}

		std::string uiFilename = TheProject->GetUIFilename();
		if (!uiFilename.empty())
		{
			LoadUI(uiFilename);
		}

		mProjectFilePath = pathname;

		return true;
	}
	else
	{
		Project::Destory();
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveProject()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(mProjectFilePath))
			{
				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(mProjectFilePath);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveProjectAs(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(pathname))
			{
				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(pathname);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseProject()
{
	URDoManager::GetSingleton().Clear();

	Project *oldProj = Project::GetSingletonPtr();
	if (!oldProj) return;

	Play(Application::PT_NONE);

	CloseScene();
	CloseUI();

	Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	std::string callFilename = "Data/" + mBoostProjectName + "/scripts/lua/end.lua";
	PX2_SC_LUA->CallFile(callFilename.c_str());

	std::string callFilenameAS = "Data/" + mBoostProjectName + "/scripts/as/end.as";
	PX2_SC_AS->CallFileFunction(callFilenameAS.c_str(), "void end()");

	std::string debugTag = "";
#if defined (_DEBUG) 
	debugTag = "D";
#endif

	std::string projName = PX2_PROJ.GetName();
	std::string folder = "Projects/" + projName + "/";
	std::string projDllPath = folder + projName + debugTag + ".dll";

	if (PX2_RM.IsFloderExist("", folder))
	{
		PX2_PLUGINMAN.Unload(projDllPath);
	}

	Project::Destory();

	PX2_RM.ClearRes(mProjectFilePath);
	PX2_RM.ClearRes(mProjectFilePath);
	PX2_RM.Clear();
	mProjectFilePath.clear();
}
//----------------------------------------------------------------------------
void Application::NewScene()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	CloseScene();

	Scene *scene = new0 Scene();
	PX2_PROJ.SetScene(scene);
	PX2_PROJ.SetSceneFilename("");
}
//----------------------------------------------------------------------------
bool Application::LoadScene(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	if (!Project::GetSingletonPtr()) return false;

	CloseScene();

	Scene *newscene = DynamicCast<Scene>(PX2_RM.BlockLoad(pathname));
	if (newscene)
	{
		mSceneFilePath = pathname;

		Project::GetSingleton().SetScene(newscene);
		Project::GetSingleton().SetSceneFilename(pathname);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveScene(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	std::string toPath = _CalSavePath(pathname);

	if (_SaveSceneInternal(toPath))
	{
		PX2_PROJ.SetSceneFilename(toPath);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveSceneAs(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	std::string toPath = _CalSavePath(pathname);

	return _SaveSceneInternal(toPath);
}
//----------------------------------------------------------------------------
void Application::CloseScene()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (scene)
	{
		PX2_PROJ.SetScene(0);
		PX2_PROJ.SetSceneFilename("");
		PX2_RM.ClearRes(mSceneFilePath);
		mSceneFilePath.clear();
	}
}
//----------------------------------------------------------------------------
bool Application::LoadUI(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	ObjectPtr uiObj = PX2_RM.BlockLoad(pathname);
	UIFrame *ui = DynamicCast<UIFrame>(uiObj);
	if (ui)
	{
		mUIFilePath = pathname;

		Project::GetSingleton().SetUIFrame(ui);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseUI()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	PX2_PROJ.SetUIFrame(0);
	PX2_RM.ClearRes(mUIFilePath);
	mUIFilePath.clear();
}
//----------------------------------------------------------------------------
std::string Application::_CalSavePath(const std::string &pathname)
{
	std::string pathStand = StringHelp::StandardisePath(pathname);
	pathStand = pathStand.substr(0, pathStand.length() - 1);

	std::string toPath = pathStand;
	size_t begin = toPath.find("Data/");
	if (begin != std::string::npos)
	{
		size_t length = toPath.length() - begin;
		toPath = toPath.substr(begin, length);
	}

	return toPath;
}
//----------------------------------------------------------------------------
bool Application::_SaveSceneInternal(const std::string &pathname)
{
	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return false;

	scene->SetResourcePath(pathname);

	OutStream outStream;

	outStream.Insert(scene);

	if (outStream.Save(pathname))
	{
		Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedScene);
		PX2_EW.BroadcastingLocalEvent(ent);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------