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

	Project *proj = new0 Project();
	proj->SetName(projName);
	proj->SetScreenOrientation((Project::ScreenOrientation)so);
	proj->SetSize((float)width, (float)height);
	proj->Save(pathname);
	mProjectFilePath = pathname;

	Event *ent = SimuES::CreateEventX(SimuES::NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	Event *entUI = SimuES::CreateEventX(SimuES::NewUI);
	PX2_EW.BroadcastingLocalEvent(entUI);
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadProject(const std::string &pathname)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return false;

	CloseProject();

	Project *newProj = new0 Project();
	if (newProj->Load(pathname))
	{
		Event *event = SimuES::CreateEventX(SimuES::LoadedProject);
		EventWorld::GetSingleton().BroadcastingLocalEvent(event);

		const std::string &sceneFilename = newProj->GetSceneFilename();
		if (!sceneFilename.empty())
		{
			LoadScene(sceneFilename);
		}

		const std::string &uiFilename = newProj->GetUIFilename();
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
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return;

	PX2_EDIT.Reset();

	CloseScene();
	CloseUI();

	Project *oldProj = Project::GetSingletonPtr();
	if (oldProj)
	{
		Event *ent = SimuES::CreateEventX(SimuES::CloseProject);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

		Project::Destory();

		PX2_RM.ClearRes(mProjectFilePath);
		mProjectFilePath.clear();
	}
}
//----------------------------------------------------------------------------
void EngineLoop::NewScene()
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return;

	CloseScene();

	Scene *scene = new0 Scene();
	PX2_PROJ.SetScene(scene);
	PX2_PROJ.SetSceneFilename("");

	Event *ent = SimuES::CreateEventX(SimuES::NewScene);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadScene(const std::string &pathname)
{
	bool canDoChange = (EngineLoop::PT_NONE == PX2_ENGINELOOP.GetPlayType());
	if (!canDoChange) return false;

	CloseScene();

	Scene *newscene = DynamicCast<Scene>(PX2_RM.BlockLoad(pathname));
	if (newscene)
	{
		Project::GetSingleton().SetScene(newscene);
		Project::GetSingleton().SetSceneFilename(pathname);

		Event *ent = SimuES::CreateEventX(SimuES::LoadedScene);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

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
		PX2_RM.ClearRes(scene->GetResourcePath());
		scene = 0;

		Event *ent = SimuES::CreateEventX(SimuES::CloseScene);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
bool EngineLoop::LoadUI(const std::string &pathname)
{
	ObjectPtr uiObj = PX2_RM.BlockLoad(pathname);
	UIFrame *ui = DynamicCast<UIFrame>(uiObj);
	if (ui)
	{
		Project::GetSingleton().SetUIFrame(ui);

		Event *eventUI = SimuES::CreateEventX(SimuES::LoadedUI);
		EventWorld::GetSingleton().BroadcastingLocalEvent(eventUI);

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

	Event *eventUI = SimuES::CreateEventX(SimuES::CloseUI);
	EventWorld::GetSingleton().BroadcastingLocalEvent(eventUI);
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