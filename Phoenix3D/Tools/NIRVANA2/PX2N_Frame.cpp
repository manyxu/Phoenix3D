// PX2N_Frame.cpp

#include "PX2N_Frame.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2InputManager.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2N_DlgCreateProject.hpp"
using namespace NA;
using namespace PX2;

const int sID_ENGINELOOPTIMER = PX2_EDIT_GETID;

BEGIN_EVENT_TABLE(N_Frame, wxFrame)
EVT_TIMER(sID_ENGINELOOPTIMER, N_Frame::OnTimer)
EVT_SIZE(N_Frame::OnSize)
EVT_ENTER_WINDOW(N_Frame::OnEnterWindow)
EVT_LEAVE_WINDOW(N_Frame::OnLeaveWindow)
EVT_LEFT_DOWN(N_Frame::OnLeftDown)
EVT_LEFT_UP(N_Frame::OnLeftUp)
EVT_MOTION(N_Frame::OnMotion)
END_EVENT_TABLE();

//----------------------------------------------------------------------------
N_Frame::N_Frame(wxWindow *parent, int id, const std::string &name,
	const std::string &title, int xPos,	int yPos, int width, int height, 
	long style) :
	wxFrame((wxFrame*)parent, -1, title, wxPoint(xPos, yPos),
	wxSize(width, height), style),
	mIsInitlized(false),
	mIsSized(false),
	mRenderView(0),
	mMainMenuBar(0),
	mIsExit(false),
	mIsNewProject(false),
	mIsOpenProject(false),
	mIsSaveProject(false),
	mIsCloseProject(false),
	mIsNewScene(false),
	mIsOpenScene(false),
	mIsSaveScene(false),
	mIsSaveSceneAs(false),
	mIsCloseScene(false)
{
	SetName(name);
	PX2_EW.ComeIn(this);

	mTimer.SetOwner(this, sID_ENGINELOOPTIMER);
	mTimer.Start(25);

	mRenderView = new RenderView(id, this);
}
//----------------------------------------------------------------------------
RenderView *N_Frame::GerRenderView()
{
	return mRenderView;
}
//----------------------------------------------------------------------------
N_Frame::~N_Frame()
{
	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void N_Frame::DoExecute(Event *event)
{
	if (EditEventSpace::IsEqual(event, EditEventSpace::N_Window))
	{
		if ("Main" == GetName())
		{
			CreatePopUpWindow("abcd");
		}
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_AddMenu))
	{
		EED_AddMenu data = event->GetData<EED_AddMenu>();
		if (data.Where == GetName())
		{
			if (EED_AddMenu::IT_MAIN_MENU == data.TheItemType)
			{
				if (0 == mMainMenuBar)
					_CreateMenu();

				wxMenu *menu = AddMainMenuItem(data.Title);
				mMenuMap[data.Name] = menu;
			}
			else
			{
				std::map<std::string, wxMenu*>::iterator it =
					mMenuMap.find(data.ParentName);
				if (it != mMenuMap.end())
				{
					wxMenu *menu = it->second;

					if (EED_AddMenu::IT_SUB_MENU == data.TheItemType)
					{
						wxMenu *subMenu = AddSubMenuItem(menu, data.Title);
						mMenuMap[data.ParentName + data.Name] = subMenu;
					}
					else if (EED_AddMenu::IT_ITEM == data.TheItemType)
					{
						AddMenuItem(menu, data.Title, data.Script, data.Tag);
					}
					else if (EED_AddMenu::IT_ITEMSPARATER == data.TheItemType)
					{
						AddSeparater(menu);
					}
				}
			}
		}
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_NewProject))
	{
		mIsNewProject = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_OpenProject))
	{
		mIsOpenProject = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_SaveProject))
	{
		mIsSaveProject = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_CloseProject))
	{
		mIsCloseProject = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_NewScene))
	{
		mIsNewScene = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_OpenScene))
	{
		mIsOpenScene = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_SaveScene))
	{
		mIsSaveScene = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_SaveSceneAs))
	{
		mIsSaveSceneAs = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_CloseScene))
	{
		mIsCloseScene = true;
	}
	else if (EditEventSpace::IsEqual(event, EditEventSpace::N_Exit))
	{
		mIsExit = true;
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoNewProject()
{
	mIsNewProject = false;

	DlgCreateProject dlg(this);

	if (wxID_OK == dlg.ShowModal())
	{
		std::string name = dlg.mProjName;
		int screenOriention = dlg.mScreenOrientation;
		int width = dlg.mProjWidth;
		int height = dlg.mProjHeight;

		std::string pathName = name + "/";

		bool isProjectFolderExist = PX2_RM.IsFloderExist("Data/", pathName);
		if (isProjectFolderExist)
		{
			MessageBox(PX2_LMVAL("Notice"), PX2_LMVAL("TipCreateProject"), 0);
			return;
		}
		else
		{
			PX2_RM.CreateFloder("Data/", pathName);
			PX2_RM.CreateFloder("Data/", pathName + "images/");
			PX2_RM.CreateFloder("Data/", pathName + "models/");
			PX2_RM.CreateFloder("Data/", pathName + "scenes/");
			PX2_RM.CreateFloder("Data/", pathName + "scripts/");
			std::string scriptPath = "Data/" + pathName + "scripts/" + "start.lua";
			fopen(scriptPath.c_str(), "wb");

			std::string path = "Data/" + pathName + name + ".px2proj";
			PX2_ENGINELOOP.NewProject(path, name, screenOriention,
				width, height);
		}
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoOpenProject()
{
	mIsOpenProject = false;

	wxFileDialog dlg(this,
		wxT("Open project"),
		wxEmptyString,
		wxEmptyString,
		wxT("Project (*.px2proj)|*.px2proj"));

	dlg.CenterOnParent();

	if (dlg.ShowModal() == wxID_OK)
	{
		std::string path = dlg.GetPath();
		path = StringHelp::StandardiseFilename(path);
		PX2_ENGINELOOP.LoadProject(path);
	}
	else
	{
		dlg.Close();
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoSaveProject()
{
	mIsSaveProject = false;

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (proj->GetScene())
		{
			DoSaveScene();
		}
	}

	PX2_ENGINELOOP.SaveProject();
}
//----------------------------------------------------------------------------
void N_Frame::DoCloseProject()
{
	mIsCloseProject = false;

	PX2_ENGINELOOP.CloseProject();
}
//----------------------------------------------------------------------------
void N_Frame::DoNewScene()
{
	mIsNewScene = false;
	PX2_ENGINELOOP.NewScene();
}
//----------------------------------------------------------------------------
void N_Frame::DoOpenScene()
{
	mIsOpenScene = false;

	wxFileDialog dlg(this,
		wxT("Open scene"),
		wxEmptyString,
		wxEmptyString,
		wxT("scene (*.px2obj)|*.px2obj"));

	dlg.CenterOnParent();

	if (wxID_OK == dlg.ShowModal())
	{
		std::string strPath = dlg.GetPath();
		PX2_ENGINELOOP.LoadScene(strPath);
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoSaveScene()
{
	mIsSaveScene = false;

	std::string path;
	if (Project::GetSingletonPtr())
		path = Project::GetSingleton().GetSceneFilename();

	if (!path.empty())
	{
		PX2_ENGINELOOP.SaveScene(path.c_str());
	}
	else
	{
		wxFileDialog dlg(this,
			wxT("Save scene"),
			wxEmptyString,
			wxEmptyString,
			wxT("scene (*.px2obj)|*.px2obj"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		dlg.CenterOnParent();

		if (wxID_OK == dlg.ShowModal())
		{
			std::string strPath = dlg.GetPath();
			PX2_ENGINELOOP.SaveScene(strPath);
		}
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoSaveSceneAs()
{
	mIsSaveSceneAs = false;

	wxFileDialog dlg(this,
		wxT("Save scene as"),
		wxEmptyString,
		wxEmptyString,
		wxT("scene (*.px2obj)|*.px2obj"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	dlg.CenterOnParent();

	if (dlg.ShowModal() == wxID_OK)
	{
		std::string strPath = dlg.GetPath();
		PX2_ENGINELOOP.SaveSceneAs(strPath);
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoCloseScene()
{
	mIsCloseScene = false;

	EditMap *map = PX2_EDIT.GetEditMap();
	if (map)
	{
		PX2_ENGINELOOP.CloseScene();
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoExit()
{
	mIsExit = false;

	int ret = wxMessageBox(PX2_LM.GetValue("Notice"), PX2_LM.GetValue("Notice"),
		wxYES_NO);
	if (wxYES == ret)
	{
		wxExit();
	}
}
//----------------------------------------------------------------------------
void N_Frame::OnTimer(wxTimerEvent& e)
{
	PX2_UNUSED(e);

	if (mIsNewProject)
	{
		DoNewProject();
	}
	if (mIsOpenProject)
	{
		DoOpenProject();
	}
	if (mIsSaveProject)
	{
		DoSaveProject();
	}
	if (mIsCloseProject)
	{
		DoCloseProject();
	}
	if (mIsNewScene)
	{
		DoNewScene();
	}
	if (mIsOpenScene)
	{
		DoOpenScene();
	}
	if (mIsSaveScene)
	{
		DoSaveScene();
	}
	if (mIsSaveSceneAs)
	{
		DoSaveSceneAs();
	}
	if (mIsCloseScene)
	{
		DoCloseScene();
	}
	if (mIsExit)
	{
		DoExit();
	}
}
//----------------------------------------------------------------------------
void N_Frame::OnSize(wxSizeEvent& e)
{
	PX2_UNUSED(e);

	wxSize size = GetClientSize();
	if (mRenderView)
		mRenderView->SetSize(0, 0, size.x, size.y);

	mIsSized = true;
}
//----------------------------------------------------------------------------
void N_Frame::OnEnterWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_Frame::OnLeaveWindow(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_Frame::OnLeftDown(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_Frame::OnLeftUp(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
void N_Frame::OnMotion(wxMouseEvent& e)
{
	PX2_UNUSED(e);
}
//----------------------------------------------------------------------------
RenderView *N_Frame::GetMainRenderView()
{
	return mRenderView;
}
//----------------------------------------------------------------------------