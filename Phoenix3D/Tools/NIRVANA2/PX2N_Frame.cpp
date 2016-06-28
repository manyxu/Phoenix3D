// PX2N_Frame.cpp

#include "PX2N_Frame.hpp"
#include "PX2Edit.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2InputManager.hpp"
#include "PX2EU_Manager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2N_DlgCreateProject.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2GraphicsEventType.hpp"
using namespace NA;
using namespace PX2;

BEGIN_EVENT_TABLE(N_Frame, wxFrame)
EVT_SIZE(N_Frame::OnSize)
END_EVENT_TABLE();

//----------------------------------------------------------------------------
N_Frame::N_Frame(PX2::RenderWindow *uiWindow, wxWindow *parent, long style) :
wxFrame((wxFrame*)parent, -1, uiWindow->GetTitle(), wxPoint(0, 0),
wxSize((int)uiWindow->GetScreenSize().Width, 
(int)uiWindow->GetScreenSize().Height), style),
mRenderWindow(uiWindow)
{
	_Init();

	std::string name = uiWindow->GetName();
	std::string title = uiWindow->GetTitle();
	int width = (int)uiWindow->GetScreenSize().Width;
	int height = (int)uiWindow->GetScreenSize().Height;

	SetName(name);
	PX2_EW.ComeIn(this);

	mTimerID = PX2_EDIT_GETID;
	Connect(mTimerID, wxEVT_TIMER, wxTimerEventHandler(N_Frame::OnTimer));
	mTimer.SetOwner(this, mTimerID);
	mTimer.Start(25);

	SetFocus();

	mRenderView = new RenderView(uiWindow, this);

	if ("Main" != name)
	{
		Renderer *renderer = PX2_ENGINELOOP.CreateRenderer(name, 
			mRenderView->GetHandle(), width, height, 0);
		uiWindow->SetRenderer(renderer);
		uiWindow->SetScreenSize(Sizef((float)width, (float)height));
	}

	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer(wxVERTICAL);
	bSizer69->Add(mRenderView, 1, wxEXPAND, 5);
	this->SetSizer(bSizer69);
	this->Layout();
}
//----------------------------------------------------------------------------
void N_Frame::_Init()
{
	mIsInitlized = false;
	mIsSized = false;
	mRenderView = 0;
	mMainMenuBar = 0;
	mIsExit = false;
	mIsNewProject = false;
	mIsOpenProject = false;
	mIsSaveProject = false;
	mIsCloseProject = false;
	mIsNewScene = false;
	mIsOpenScene = false;
	mIsSaveScene = false;
	mIsSaveSceneAs = false;
	mIsCloseScene = false;
	mIsPopUpRightMenu = false;
	mEditMenu = 0;
	mIsNWindow = false;
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

	if (mEditMenu)
	{
		delete mEditMenu;
		mEditMenu = 0;
	}
}
//----------------------------------------------------------------------------
void N_Frame::DoExecute(Event *event)
{
	std::string name = GetName();
	if ("Main" == name)
	{
		if (EditEventSpace::IsEqual(event, EditEventSpace::N_Window))
		{
			mIsNWindow = true;
			mNUIWindow = event->GetData<RenderWindow*>();
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
				else if (EED_AddMenu::IT_MAIN_SUBMENU == data.TheItemType ||
					EED_AddMenu::IT_MAIN_ITEM == data.TheItemType ||
					EED_AddMenu::IT_MAIN_ITEMSPARATER == data.TheItemType)
				{
					std::map<std::string, wxMenu*>::iterator it =
						mMenuMap.find(data.ParentName);
					if (it != mMenuMap.end())
					{
						wxMenu *menu = it->second;

						if (EED_AddMenu::IT_MAIN_SUBMENU == data.TheItemType)
						{
							wxMenu *subMenu = AddSubMenuItem(menu, data.Title);
							mMenuMap[data.ParentName + data.Name] = subMenu;
						}
						else if (EED_AddMenu::IT_MAIN_ITEM == data.TheItemType)
						{
							AddMenuItem(menu, data.Title, data.Script, data.Tag);
						}
						else if (EED_AddMenu::IT_MAIN_ITEMSPARATER == data.TheItemType)
						{
							AddSeparater(menu);
						}
					}
				}
			}

			if (EED_AddMenu::IT_EDIT_MENU == data.TheItemType ||
				EED_AddMenu::IT_EDIT_SUBMENU == data.TheItemType ||
				EED_AddMenu::IT_EDIT_ITEM == data.TheItemType ||
				EED_AddMenu::IT_EDIT_ITEMSPARATER == data.TheItemType ||
				EED_AddMenu::IT_EDIT_POPUP == data.TheItemType)
			{
				if (EED_AddMenu::IT_EDIT_MENU == data.TheItemType)
				{
					wxMenu *menu = CreateEditMenu();
					mMenuMap_Edit[data.Name] = menu;
				}
				else
				{
					std::map<std::string, wxMenu*>::iterator it =
						mMenuMap_Edit.find(data.ParentName);
					if (it != mMenuMap_Edit.end())
					{
						wxMenu *menu = it->second;

						if (EED_AddMenu::IT_EDIT_SUBMENU == data.TheItemType)
						{
							if (menu)
							{
								wxMenu *subMenu = AddSubMenuItem(menu, data.Title);
								mMenuMap_Edit[data.ParentName + data.Name] = subMenu;
							}
						}
						else if (EED_AddMenu::IT_EDIT_ITEM == data.TheItemType)
						{
							if (menu)
							{
								AddMenuItem(menu, data.Title, data.Script, data.Tag);
							}
						}
						else if (EED_AddMenu::IT_EDIT_ITEMSPARATER == data.TheItemType)
						{
							if (menu)
							{
								AddSeparater(menu);
							}
						}
					}

					if (EED_AddMenu::IT_EDIT_POPUP == data.TheItemType)
					{
						mPopUpRightMenuPos = data.PopUpPos;
						mIsPopUpRightMenu = true;
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
		else if (EditEventSpace::IsEqual(event, EditEventSpace::N_PlayTip))
		{
			EED_Tip tipData = event->GetData<EED_Tip>();
			
			MessageBox(tipData.Title, tipData.Content, 0);
		}
	}

	if (GraphicsES::IsEqual(event, GraphicsES::WindowMaxSize))
	{
		RenderWindow *rw = event->GetData<RenderWindow*>();
		if (rw == mRenderWindow)
		{
			Maximize(mRenderWindow->IsMaxSize());
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::WindowClose))
	{
		RenderWindow *rw = event->GetData<RenderWindow*>();
		if (rw == mRenderWindow)
		{
			Show(false);
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::WindowSetPosition))
	{
		RenderWindow *rw = event->GetData<RenderWindow*>();
		if (rw == mRenderWindow && !rw->IsMain())
		{
			const APoint &pos = rw->GetPosition();
			wxPoint pt;
			pt.x = pos.X();
			pt.y = GetSize().GetHeight() - pos.Z();
			SetPosition(pt);
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::WindowSetCursorType))
	{
		RenderWindow::CursorType ct = RenderWindow::GetCursorType();
		
		wxStockCursor wxCursorType = (wxStockCursor)ct;
		SetCursor(wxCursor(wxCursorType));
	}
}
//----------------------------------------------------------------------------
void _CreateScriptFile(const std::string &pathName,
	const std::string &scFileName)
{
	std::string scriptPath = "Data/" + pathName + "scripts/" + scFileName;
	std::ofstream outputFile;
	outputFile.open(scriptPath.c_str());
	std::string scriptStart;
	scriptStart += "-- " + scFileName;
	outputFile << scriptStart;
	outputFile.close();
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
			PX2_RM.CreateFloder("Data/", pathName + "scripts/bp");

			_CreateScriptFile(pathName, "start.lua");
			_CreateScriptFile(pathName, "end.lua");

			std::string path = "Data/" + pathName + name + ".px2proj";
			PX2_ENGINELOOP.NewProject(path, name, screenOriention, width,
				height);
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

	PX2_ENGINELOOP.CloseScene();
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

	std::string name = GetName();
	if ("Main" == name)
	{
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
		if (mIsNWindow)
		{
			mIsNWindow = false;
			CreatePopUpWindow(mNUIWindow);
		}
	}

	if (mIsPopUpRightMenu && mEditMenu)
	{
		PopUpRightMenu(mPopUpRightMenuPos.X(), mSize.GetHeight() -
			mPopUpRightMenuPos.Z());
	}
}
//----------------------------------------------------------------------------
void N_Frame::OnSize(wxSizeEvent& e)
{
	PX2_UNUSED(e);

	mSize = GetClientSize();
	if (mRenderView)
		mRenderView->SetSize(0, 0, mSize.x, mSize.y);

	mIsSized = true;
}
//----------------------------------------------------------------------------
RenderView *N_Frame::GetMainRenderView()
{
	return mRenderView;
}
//----------------------------------------------------------------------------