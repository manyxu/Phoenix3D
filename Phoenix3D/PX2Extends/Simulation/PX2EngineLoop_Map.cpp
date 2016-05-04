// PX2EngineLoop_Map.cpp

#include "PX2EngineLoop.hpp"
#include "PX2SimulationEventType.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void EngineLoop::NewProject(const std::string &pathname,
	const std::string &projName, int so, int width, int height)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return;

	CloseProject();

	TheProject = new0 Project();
	TheProject->SetName(projName);
	TheProject->SetScreenOrientation((Project::ScreenOrientation)so);
	TheProject->SetSize((float)width, (float)height);
	TheProject->Save(pathname);
	mProjectFilePath = pathname;

	Event *ent = SimuES::CreateEventX(SimuES::NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	Event *entUI = SimuES::CreateEventX(SimuES::NewUI);
	PX2_EW.BroadcastingLocalEvent(entUI);

	Event *entBP = SimuES::CreateEventX(SimuES::NewBP);
	PX2_EW.BroadcastingLocalEvent(entBP);
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadProject(const std::string &pathname)
{
	CloseProject();

	TheProject = new0 Project();
	if (TheProject->Load(pathname))
	{
		if (ScriptManager::GetSingletonPtr())
		{
			PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", Project::GetSingletonPtr());
		}

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

		Event *event = SimuES::CreateEventX(SimuES::LoadedProject);
		EventWorld::GetSingleton().BroadcastingLocalEvent(event);

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

		const std::string &bpFilename = TheProject->GetBPFilename();
		if (!bpFilename.empty())
		{
			//LoadBP(bpFilename);
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
bool EngineLoop::SaveProject()
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(mProjectFilePath))
			{
				Event *ent = SimuES::CreateEventX(
					SimuES::SavedProject);
				EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

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
bool EngineLoop::SaveProjectAs(const std::string &pathname)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(pathname))
			{
				Event *ent = SimuES::CreateEventX(
					SimuES::SavedProject);
				EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

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
void EngineLoop::CloseProject()
{
	Project *oldProj = Project::GetSingletonPtr();
	if (!oldProj) return;

	Play(EngineLoop::PT_NONE);

	CloseScene();
	CloseUI();
	CloseBP();

	PX2_EDIT.Reset();

	Event *ent = SimuES::CreateEventX(SimuES::CloseProject);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

	std::string callFilename = "Data/" + mProjectName + "/scripts/lua/end.lua";
	PX2_SC_LUA->CallFile(callFilename.c_str());

	std::string callFilenameAS = "Data/" + mProjectName + "/scripts/as/end.as";
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
void EngineLoop::NewScene()
{
	CloseScene();

	Scene *scene = new0 Scene();
	PX2_PROJ.SetScene(scene);
	PX2_PROJ.SetSceneFilename("");
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadScene(const std::string &pathname)
{
	if (!Project::GetSingletonPtr())
		return false;

	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return false;

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
bool EngineLoop::SaveScene(const std::string &pathname)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
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
bool EngineLoop::SaveSceneAs(const std::string &pathname)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return false;

	std::string toPath = _CalSavePath(pathname);

	return _SaveSceneInternal(toPath);
}
//----------------------------------------------------------------------------
void EngineLoop::CloseScene()
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
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
bool EngineLoop::LoadUI(const std::string &pathname)
{
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
void EngineLoop::CloseUI()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	PX2_PROJ.SetUIFrame(0);
	PX2_RM.ClearRes(mUIFilePath);
	mUIFilePath.clear();
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadBP(const std::string &pathname)
{
	ObjectPtr bpObj = PX2_RM.BlockLoad(pathname);
	BPPackage *bpPackage = DynamicCast<BPPackage>(bpObj);
	if (bpPackage)
	{
		mBPFilePath = pathname;

		Project::GetSingleton().SetBPPackage(bpPackage);

		Event *eventBP = SimuES::CreateEventX(SimuES::LoadedBP);
		EventWorld::GetSingleton().BroadcastingLocalEvent(eventBP);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineLoop::CloseBP()
{
	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	PX2_PROJ.SetBPPackage(0);
	PX2_RM.ClearRes(mBPFilePath);
	mBPFilePath.clear();

	Event *eventBP = SimuES::CreateEventX(SimuES::CloseBP);
	PX2_EW.BroadcastingLocalEvent(eventBP);
}
//----------------------------------------------------------------------------
std::string EngineLoop::_CalSavePath(const std::string &pathname)
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
bool EngineLoop::_SaveSceneInternal(const std::string &pathname)
{
	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return false;

	scene->SetResourcePath(pathname);

	OutStream outStream;

	outStream.Insert(scene);

	if (outStream.Save(pathname))
	{
		Event *ent = SimuES::CreateEventX(SimuES::SavedScene);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------