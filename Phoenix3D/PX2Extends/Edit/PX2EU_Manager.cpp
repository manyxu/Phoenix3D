// PX2EU_Manager.cpp

#include "PX2EU_Manager.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2EU_ProjectFrame.hpp"
#include "PX2EU_ResFrame.hpp"
#include "PX2EU_InspectorFrame.hpp"
#include "PX2EU_ConsoleFrame.hpp"
#include "PX2EU_BluePrintFrame.hpp"
#include "PX2EU_CreateFrame.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2UIWindow.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EU_Manager::EU_Manager()
{
	mNextViewID = 1;
}
//----------------------------------------------------------------------------
EU_Manager::~EU_Manager()
{
}
//----------------------------------------------------------------------------
bool EU_Manager::Initlize()
{
	CreateUIWindowMain();

	PX2_SM.SetUserTypePointer("PX2EU_MAN", "EU_Manager", this);
	PX2_SM.CallFile("DataNIRVANA2/scripts/start.lua");

	return true;
}
//----------------------------------------------------------------------------
bool EU_Manager::Terminate()
{
	mUICanvas_Main = 0;
	mFrame_Main = 0;
	mFrame_StatusBar = 0;

	mWindowMap.clear();

	return true;
}
//----------------------------------------------------------------------------
UIWindow *EU_Manager::CreateUIWindow(const std::string &name,
	const std::string title, bool isFloat, bool isMain)
{
	//UIWindow *window = new0 UIWindow();
	//window->SetFloat(isFloat);
	//window->SetMain(isMain);

	//mWindowMap[name] = window;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Window);
	PX2_EW.BroadcastingLocalEvent(ent);

	return 0;
}
//----------------------------------------------------------------------------
int EU_Manager::GetViewID(const std::string &viewName)
{
	std::map<std::string, int>::iterator it = mViewIDMap.find(viewName);
	if (it != mViewIDMap.end())
	{
		return it->second;
	}
	else
	{
		int viewID = mNextViewID;
		mViewIDMap[viewName] = viewID;
		mNextViewID++;

		return viewID;
	}
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Main_AddMainItem(const std::string &name, 
	const std::string &title)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_MAIN_MENU;
	data.Name = name;
	data.Title = title;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Main_AddSubItem(const std::string &parentName,
	const std::string &name, const std::string &title)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_SUB_MENU;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Main_AddItem(const std::string &parentName,
	const std::string &name,
	const std::string &title, const std::string &script,
	const std::string &tag)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_ITEM;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;
	data.Script = script;
	data.Tag = tag;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Main_AddItemSeparater(const std::string &parentName)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_ITEMSPARATER;
	data.ParentName = parentName;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::NewProject()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::OpenProject()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_OpenProject);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::SaveProject()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_SaveProject);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::CloseProject()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_CloseProject);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::NewScene()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_NewScene);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::OpenScene()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_OpenScene);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::SaveScene()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_SaveScene);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::SaveSceneAs()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_SaveSceneAs);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::CloseScene()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_CloseScene);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Exit()
{
	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Exit);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateUIWindowMain()
{
	std::string name = "Main";
	int id = GetViewID(name);

	// window
	mUIWindowMain = new0 UIWindow();
	mUIWindowMain->SetName("Main");
	mUIWindowMain->SetFloat(false);
	mUIWindowMain->SetMain(true);
	mUIWindowMain->SetID(id);
	mWindowMap["Main"] = mUIWindowMain;

	// main
	mUICanvas_Main = new0 UICanvas(id);
	mUICanvas_Main->SetName("EU_Main");
	mUICanvas_Main->SetRenderer(Renderer::GetDefaultRenderer());
	PX2_GR.AddCanvas("EU_Main", mUICanvas_Main);
	mUIWindowMain->SetUICanvas(mUICanvas_Main);

	// main frame
	CreateFrame_Main();

	// content
	CreateFrame_Content();

	// status bar
	CreateFrame_StatusBar();

	// project res bar
	CreateFrame_ProjectRes();

	// inspector bar
	CreateFrame_Inspector();

	// console frame
	CreateFrame_Console();

	// center
	CreateFrame_Center();
}
//----------------------------------------------------------------------------
UIWindow *EU_Manager::GetUIWindowMain()
{
	return mUIWindowMain;
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Main()
{
	mFrame_Main = new0 UIFrame();
	mUICanvas_Main->AttachChild(mFrame_Main);
	mFrame_Main->SetSize(500.0f, 300.0f);
	mFrame_Main->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_Main->SetAnchorVer(Float2(0.0f, 1.0f));
	mFrame_Main->SetPivot(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
UIFrame *EU_Manager::GetFrame_Main()
{
	return mFrame_Main;
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Content()
{
	mFrame_Content = new0 UIAuiFrame();
	mFrame_Main->AttachChild(mFrame_Content);
	mFrame_Content->SetName("ContentFrame");
	UIAuiManager::GetSingleton().SetAuiFrame(mFrame_Content);

	mFrame_Content->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_Content->SetAnchorVer(Float2(0.0f, 1.0f));
	mFrame_Content->SetAnchorParamVer(Float2(PX2_UISM.Size_StatusBar, 0.0f));
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Center()
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame(
		mFrame_Content->AuiBlockDefault, UILPT_CENTER);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	UIFrame *frame = new0 UIFrame();
	tabFrame->AddTab("Stage", frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	EU_BluePrintFrame *frame1 = new0 EU_BluePrintFrame();
	tabFrame->AddTab("BluePrint", frame1);
	frame1->SetAnchorHor(0.0f, 1.0f);
	frame1->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Stage");
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_StatusBar()
{
	mFrame_StatusBar = new0 UIFrame();
	mFrame_Main->AttachChild(mFrame_StatusBar);
	mFrame_StatusBar->SetName("StatusBar");

	UIPicBox *picBox = mFrame_StatusBar->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(PX2_UISM.Color_StatusBar);

	mFrame_StatusBar->SetSize(0.0f, PX2_UISM.Size_StatusBar);

	mFrame_StatusBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_StatusBar->SetAnchorVer(Float2(0.0f, 0.0f));
	mFrame_StatusBar->SetPivot(0.5f, 0.0f);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_ProjectRes()
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame(
		mFrame_Content->AuiBlockDefault, UILPT_LEFT);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	tabFrame->SetAnchorHor(0.0f, 1.0f);
	tabFrame->SetAnchorVer(0.0f, 1.0f);

	EU_ProjectFrame *frameProj = new0 EU_ProjectFrame();
	tabFrame->AddTab("Project", frameProj);
	frameProj->SetAnchorHor(0.0f, 1.0f);
	frameProj->SetAnchorVer(0.0f, 1.0f);

	EU_CreateFrame *frameCreate = new0 EU_CreateFrame();
	tabFrame->AddTab("Create", frameCreate);
	frameCreate->SetAnchorHor(0.0f, 1.0f);
	frameCreate->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Project");
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Inspector()
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame(
		mFrame_Content->AuiBlockDefault, UILPT_RIGHT);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_InspectorFrame *frame = new0 EU_InspectorFrame();
	tabFrame->AddTab("Property", frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Property");
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Console()
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame(
		mFrame_Content->AuiBlockDefault, UILPT_BOTTOM);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_ResFrame *frame1 = new0 EU_ResFrame();
	tabFrame->AddTab("Resource", frame1);
	frame1->SetAnchorHor(0.0f, 1.0f);
	frame1->SetAnchorVer(0.0f, 1.0f);

	EU_ConcoleFrame *frame = new0 EU_ConcoleFrame();
	tabFrame->AddTab("Console", frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Console");
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateUIAuiBlockFrame(UIAuiBlockFrame *parent,
	UILayoutPosType pos, const Sizef &size)
{
	UIAuiBlockFrame *blockFrame = parent->CreateAddPosFrame(pos, 
		Sizef(250.0f, 250.0f));

	blockFrame->SetName(PX2_UIAUIM.GenName("UIAuiBlockFrame"));

	UITabFrame *tabFrame = new0 UITabFrame();
	blockFrame->AttachChild(tabFrame);
	tabFrame->SetName("UITabFrame_" + blockFrame->GetName());

	PX2_UIAUIM.AddTabFrame(tabFrame->GetName(), tabFrame);

	UIPicBox *backPicBox = blockFrame->CreateAddBackgroundPicBox();
	backPicBox->SetTexture("Data/engine/white.png");
	backPicBox->SetColor(PX2_UISM.Color_ViewBackground);

	return blockFrame;
}
//----------------------------------------------------------------------------