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
#include "PX2EU_StageFrame.hpp"
#include "PX2EU_PreViewFrame.hpp"
#include "PX2EU_TimeLineFrame.hpp"
#include "PX2EU_SimuFrame.hpp"
#include "PX2EU_ResGridItem.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EU_Manager::EU_Manager()
{
}
//----------------------------------------------------------------------------
EU_Manager::~EU_Manager()
{
}
//----------------------------------------------------------------------------
bool EU_Manager::Initlize()
{
	CreateUIWindowMain();

	PX2_SC_LUA->SetUserTypePointer("PX2EU_MAN", "EU_Manager", this);
	PX2_SC_LUA->CallFile("DataNIRVANA2/scripts/start.lua");

	return true;
}
//----------------------------------------------------------------------------
bool EU_Manager::Terminate()
{
	EU_ResGridItem::sLastSelectItemBut = 0;

	mFrame_Main = 0;
	mFrame_ToolBar = 0;
	mFrame_StatusBar = 0;

	return true;
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
	data.TheItemType = EED_AddMenu::IT_MAIN_SUBMENU;
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
	data.TheItemType = EED_AddMenu::IT_MAIN_ITEM;
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
	data.TheItemType = EED_AddMenu::IT_MAIN_ITEMSPARATER;
	data.ParentName = parentName;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Edit_Begin(const std::string &whe, const std::string &name)
{
	EED_AddMenu data;
	data.Where = whe;
	data.Name = name;
	data.TheItemType = EED_AddMenu::IT_EDIT_MENU;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Edit_AddSubItem(const std::string &whe,
	const std::string &parentName,
	const std::string &name, const std::string &title)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_SUBMENU;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Edit_AddItem(const std::string &whe,
	const std::string &parentName, const std::string &name, 
	const std::string &title,
	const std::string &script,
	const std::string &tag)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_ITEM;
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
void EU_Manager::Menu_Edit_AddItemSeparater(const std::string &whe,
	const std::string &parentName)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_ITEMSPARATER;
	data.ParentName = parentName;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::Menu_Edit_EndPopUp(const std::string &whe,
	const APoint &pos)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_POPUP;
	data.PopUpPos = pos;

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
void EU_Manager::PlayTip(const std::string &title, const std::string &content)
{
	EED_Tip tipData;
	tipData.Title = title;
	tipData.Content = content;

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_PlayTip);
	ent->SetData(tipData);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateUIWindowMain()
{
	RenderWindow *rw = PX2_GR.GetMainWindow();
	UICanvas *canvasMain = DynamicCast<UICanvas>(rw->GetCanvas("Main"));

	UIAuiFrame *uiAuiFrame = 0;
	mFrame_Main = CreateMainFrame(canvasMain, uiAuiFrame);
	mFrame_Content = uiAuiFrame;
	mFrame_Content->SetAnchorParamVer(Float2(PX2_UISM.Size_StatusBar, -PX2_UISM.Size_MainToolBar));
	UIAuiManager::GetSingleton().AddAuiFrame(rw, mFrame_Content);

	// tool bar
	CreateFrame_ToolBar();

	// status bar
	CreateFrame_StatusBar();

	// center
	UIAuiBlockFrame *centerFrame = CreateFrame_Center();
	UIAuiBlockFrame *outCenter = 0;
	UIAuiBlockFrame *projectFrame = CreateFrame_Project(centerFrame, outCenter);

	UIAuiBlockFrame *projOut = 0;
	UIAuiBlockFrame *resFrame = CreateFrame_Resource(projectFrame, projOut);

	UIAuiBlockFrame *propertyFrame = CreateFrame_Property(outCenter, outCenter);

	PX2_UIAUIM.SetActiveTableFrame("Stage");
}
//----------------------------------------------------------------------------
RenderWindow *EU_Manager::CreateUIWindow(const std::string &name,
	const std::string &title, const APoint &pos, const Sizef &size,
	bool isFloat)
{
	RenderWindow *rw = new0 RenderWindow();
	rw->SetName(name);
	rw->SetTitle(title);
	rw->SetFloat(isFloat);
	rw->SetMain(false);
	rw->SetScreenSize(size);
	rw->SetPosition(pos);
	PX2_GR.AddRenderWindow(name, rw);
	rw->SetParentRenderWindow(rw);

	UICanvas *canvas = new0 UICanvas();
	canvas->SetMain(true);
	canvas->SetAnchorHor(0.0f, 1.0f);
	canvas->SetAnchorVer(0.0f, 1.0f);
	rw->AddCanvas("Main", canvas);

	UIAuiFrame *uiFrame = 0;
	CreateMainFrame(canvas, uiFrame);
	PX2_UIAUIM.AddAuiFrame(rw, uiFrame);

	Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Window);
	ent->SetData<RenderWindow*>(rw);
	PX2_EW.BroadcastingLocalEvent(ent);

	return rw;
}
//----------------------------------------------------------------------------
UIFrame *EU_Manager::CreateMainFrame(Canvas *canvas,
	UIAuiFrame *&outAuiFrame)
{
	UIFrame *frame = new0 UIFrame();
	canvas->AttachChild(frame);
	frame->SetAnchorHor(Float2(0.0f, 1.0f));
	frame->SetAnchorVer(Float2(0.0f, 1.0f));
	frame->SetSize(500.0f, 300.0f);
	frame->SetPivot(0.5f, 0.5f);

	UIAuiFrame *auiFrame = new0 UIAuiFrame();
	frame->AttachChild(auiFrame);
	auiFrame->SetName("UIAuiFrame");
	auiFrame->SetAnchorHor(0.0f, 1.0f);
	auiFrame->SetAnchorVer(0.0f, 1.0f);
	outAuiFrame = auiFrame;

	return frame;
}
//----------------------------------------------------------------------------
UIFrame *EU_Manager::GetFrame_Main()
{
	return mFrame_Main;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateFrame_Center()
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame(
		mFrame_Content->AuiBlockDefault, UILPT_CENTER);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_StageFrame *stageFrame = new0 EU_StageFrame();
	tabFrame->AddTab("Stage", "Stage", stageFrame);
	stageFrame->SetAnchorHor(0.0f, 1.0f);
	stageFrame->SetAnchorVer(0.0f, 1.0f);

	EU_SimuFrame *simuFrame = new0 EU_SimuFrame();
	tabFrame->AddTab("Simulation", "Simulation", simuFrame);
	simuFrame->SetAnchorHor(0.0f, 1.0f);
	simuFrame->SetAnchorVer(0.0f, 1.0f);

	EU_BluePrintFrame *bpFrame = new0 EU_BluePrintFrame();
	tabFrame->AddTab("BluePrint", "BluePrint", bpFrame);
	bpFrame->SetAnchorHor(0.0f, 1.0f);
	bpFrame->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Stage");

	return blockFrame;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateFrame_Project(
	UIAuiBlockFrame *beforeCenterFrame,
	UIAuiBlockFrame *&outCenter)
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame2(
		beforeCenterFrame, UILPT_LEFT, outCenter, Sizef(400.0f, 500.0f));

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	tabFrame->SetAnchorHor(0.0f, 1.0f);
	tabFrame->SetAnchorVer(0.0f, 1.0f);

	EU_ProjectFrame *frameProj = new0 EU_ProjectFrame();
	tabFrame->AddTab("Project", "Project", frameProj);
	frameProj->SetAnchorHor(0.0f, 1.0f);
	frameProj->SetAnchorVer(0.0f, 1.0f);

	EU_CreateFrame *frameCreate = new0 EU_CreateFrame();
	tabFrame->AddTab("Create", "Create", frameCreate);
	frameCreate->SetAnchorHor(0.0f, 1.0f);
	frameCreate->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Project");

	return blockFrame;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateFrame_Property(
	UIAuiBlockFrame *beforeCenterFrame,
	UIAuiBlockFrame *&outCenter)
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame2(
		beforeCenterFrame, UILPT_RIGHT, outCenter, Sizef(400.0f, 500.0f));

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_InspectorFrame *frame = new0 EU_InspectorFrame();
	tabFrame->AddTab("Property", "Property", frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Property");

	return blockFrame;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateFrame_Resource(
	UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter)
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame2(
		beforeCenterFrame, UILPT_BOTTOM, outCenter, Sizef(400.0f, 500.0f));

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_ResFrame *frame1 = new0 EU_ResFrame();
	tabFrame->AddTab("Resource", "Resource", frame1);
	frame1->SetAnchorHor(0.0f, 1.0f);
	frame1->SetAnchorVer(0.0f, 1.0f);

	EU_ConcoleFrame *frame = new0 EU_ConcoleFrame();
	tabFrame->AddTab("Console", "Console", frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	EU_TimeLineFrame *frameTimeLine = new0 EU_TimeLineFrame();
	tabFrame->AddTab("TimeLine", "TimeLine", frameTimeLine);
	frameTimeLine->SetAnchorHor(0.0f, 1.0f);
	frameTimeLine->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("Resource");

	return blockFrame;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateFrame_PreView(
	UIAuiBlockFrame *beforeCenterFrame, UIAuiBlockFrame *&outCenter)
{
	UIAuiBlockFrame *blockFrame = CreateUIAuiBlockFrame2(
		beforeCenterFrame, UILPT_RIGHT, outCenter);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(
		blockFrame->GetObjectByName("UITabFrame_" + blockFrame->GetName()));

	EU_PreViewFrame *preView = new0 EU_PreViewFrame();
	tabFrame->AddTab("PreView", "PreView", preView);
	preView->SetAnchorHor(0.0f, 1.0f);
	preView->SetAnchorVer(0.0f, 1.0f);

	tabFrame->SetActiveTab("PreView");

	return blockFrame;
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_ToolBar()
{
	mFrame_ToolBar = new0 UIFrame();
	mFrame_Main->AttachChild(mFrame_ToolBar);
	mFrame_ToolBar->SetName("ToolBar");

	UIPicBox *picBox = mFrame_ToolBar->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_MainToolBar);

	mFrame_ToolBar->SetSize(0.0f, PX2_UISM.Size_MainToolBar);

	mFrame_ToolBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_ToolBar->SetAnchorVer(Float2(1.0f, 1.0f));
	mFrame_ToolBar->SetPivot(0.5f, 1.0f);

	// buts
	Sizef butSize = Sizef(PX2_UISM.Size_ToolBarBut*2.0f, PX2_UISM.Size_ToolBarBut);
	float butSpace = 2.0f;
	Sizef spliterSize = Sizef(4.0f,
		PX2_UISM.Size_ToolBar - 2);

	float addButPos = 0.0f;

	// edit type
	addButPos = butSize.Width / 2.0f;
	UIButton *butSimu = new0 UIButton();
	mFrame_ToolBar->AttachChild(butSimu);
	butSimu->LocalTransform.SetTranslateY(-1.0f);
	butSimu->SetAnchorHor(0.0f, 0.0f);
	butSimu->SetAnchorVer(0.5f, 0.5f);
	butSimu->SetAnchorParamHor(addButPos, 0.0f);
	butSimu->SetSize(butSize);
	butSimu->SetName("ButSimu");
	butSimu->AddVisitor(this);
	butSimu->CreateAddText("Simu");
	butSimu->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butSimu->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butPlay = new0 UIButton();
	mFrame_ToolBar->AttachChild(butPlay);
	butPlay->LocalTransform.SetTranslateY(-1.0f);
	butPlay->SetAnchorHor(0.0f, 0.0f);
	butPlay->SetAnchorVer(0.5f, 0.5f);
	butPlay->SetAnchorParamHor(addButPos, 0.0f);
	butPlay->SetSize(butSize);
	butPlay->SetName("ButPlay");
	butPlay->AddVisitor(this);
	butPlay->CreateAddText("Ply");
	butPlay->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butPlay->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butPlayInWindow = new0 UIButton();
	mFrame_ToolBar->AttachChild(butPlayInWindow);
	butPlayInWindow->LocalTransform.SetTranslateY(-1.0f);
	butPlayInWindow->SetAnchorHor(0.0f, 0.0f);
	butPlayInWindow->SetAnchorVer(0.5f, 0.5f);
	butPlayInWindow->SetAnchorParamHor(addButPos, 0.0f);
	butPlayInWindow->SetSize(butSize);
	butPlayInWindow->SetName("ButPlayInWindow");
	butPlayInWindow->AddVisitor(this);
	butPlayInWindow->CreateAddText("PlyW");
	butPlayInWindow->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butPlayInWindow->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_StatusBar()
{
	mFrame_StatusBar = new0 UIFrame();
	mFrame_Main->AttachChild(mFrame_StatusBar);
	mFrame_StatusBar->SetName("StatusBar");

	UIPicBox *picBox = mFrame_StatusBar->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_StatusBar);

	mFrame_StatusBar->SetSize(0.0f, PX2_UISM.Size_StatusBar);

	mFrame_StatusBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_StatusBar->SetAnchorVer(Float2(0.0f, 0.0f));
	mFrame_StatusBar->SetPivot(0.5f, 0.0f);
}
//----------------------------------------------------------------------------
void _RenderWindowButCallback (UIFrame *frame, UICallType type)
{
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but)
	{
		const std::string &name = frame->GetName();

		if (UICT_RELEASED == type)
		{
			if ("Max" == name)
			{
				RenderWindow *rw = but->GetUserData<RenderWindow*>("window");
				if (rw)
				{
					rw->MaxSize(!rw->IsMaxSize());
				}
			}
			else if ("Close" == name)
			{
				RenderWindow *rw = but->GetUserData<RenderWindow*>("window");
				if (rw)
				{
					rw->Close();
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void _TravelExecuteSetLayoutChanged (Movable *mov, Any *data, bool &goOn)
{
	PX2_UNUSED(goOn);

	SizeNode *sn = DynamicCast<SizeNode>(mov);
	if (sn)
	{
		sn->_MarkRelatvieChange();
	}
}
//----------------------------------------------------------------------------
void _UITabFrameCallback(UIFrame *frame, UICallType type)
{
	UITabFrame *tabFrame = DynamicCast<UITabFrame>(frame);
	if (tabFrame)
	{
		std::string actTabName = tabFrame->GetActiveTab();
		int numTabs = tabFrame->GetNumTabs();
		if (UICT_TABFRAME_DRAG_BEGIN == type)
		{
			if (!actTabName.empty())
			{
				const Sizef &tfSize = tabFrame->GetSize();
				Movable *topestParent = tabFrame->GetTopestParent();
				UICanvas *topestCanvas = DynamicCast<UICanvas>(topestParent);
				UIAuiBlockFrame *blockFrameParent = DynamicCast<UIAuiBlockFrame>(tabFrame->GetParent());
				UIAuiBlockFrame *blockFrameParentParent = DynamicCast<UIAuiBlockFrame>(blockFrameParent->GetParent());

				const APoint &curPickPos = topestCanvas->GetCurPickPos();

				UIFramePtr cotFrame = tabFrame->GetTab(actTabName);
				tabFrame->RemoveTab(actTabName);

				if (1 == numTabs)
				{
					blockFrameParentParent->RemovePosFrame(blockFrameParent->GetLayOutPosType());
				}

				RenderWindow *window = PX2EU_MAN.CreateUIWindow(actTabName,
					actTabName, curPickPos, tfSize, true);
				window->SetDraging(true);
				Canvas *canvasMain = window->GetCanvas("Main");
				UIAuiFrame *contentFrame = DynamicCast<UIAuiFrame>(
					canvasMain->GetObjectByName("UIAuiFrame"));

				UIAuiBlockFrame *auiBlockFrame = PX2EU_MAN.CreateUIAuiBlockFrame(
					contentFrame->AuiBlockDefault, UILPT_CENTER);

				UITabFrame *tabFrame = DynamicCast<UITabFrame>(
					auiBlockFrame->GetObjectByName("UITabFrame_" + auiBlockFrame->GetName()));

				tabFrame->AddTab(actTabName, actTabName, cotFrame);
				tabFrame->SetActiveTab(actTabName);

				UIFrame *titleBarFrame = tabFrame->GetTitleBarFrame();

				float leftMinus = -10.0f;

				UIButton *maxBut = new0 UIButton();
				titleBarFrame->AttachChild(maxBut);
				maxBut->SetName("Max");
				maxBut->SetSize(10.0f, 10.0f);
				maxBut->LocalTransform.SetTranslateY(-5.0f);
				maxBut->SetAnchorHor(1.0f, 1.0f);
				maxBut->SetAnchorVer(0.5f, 0.5f);
				maxBut->SetAnchorParamHor(-18.0f + leftMinus, 0.0f);
				maxBut->SetAnchorParamVer(0.0f, 0.0f);
				maxBut->SetUICallback(_RenderWindowButCallback);
				maxBut->SetUserData("window", window);

				UIButton *closeBut = new0 UIButton();
				titleBarFrame->AttachChild(closeBut);
				closeBut->SetName("Close");
				closeBut->SetSize(10.0f, 10.0f);
				closeBut->LocalTransform.SetTranslateY(-5.0f);
				closeBut->SetAnchorHor(1.0f, 1.0f);
				closeBut->SetAnchorVer(0.5f, 0.5f);
				closeBut->SetAnchorParamHor(-6.0f + leftMinus, 0.0f);
				closeBut->SetAnchorParamVer(0.0f, 0.0f);
				closeBut->SetUICallback(_RenderWindowButCallback);
				closeBut->SetUserData("window", window);

				Node::TravelExecute(auiBlockFrame, _TravelExecuteSetLayoutChanged);
			}
		}
	}
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateUIAuiBlockFrame(UIAuiBlockFrame *parent,
	UILayoutPosType pos, const Sizef &size)
{
	UIAuiBlockFrame *blockFrame = parent->CreateAddPosFrame(pos, 
		size);

	blockFrame->SetName(PX2_UIAUIM.GenName("UIAuiBlockFrame"));

	UITabFrame *tabFrame = new0 UITabFrame();
	blockFrame->AttachChild(tabFrame);
	tabFrame->LocalTransform.SetTranslateY(-1.0f);
	tabFrame->SetName("UITabFrame_" + blockFrame->GetName());
	tabFrame->SetUICallback(_UITabFrameCallback);

	PX2_UIAUIM.AddTabFrame(tabFrame->GetName(), tabFrame);

	UIPicBox *backPicBox = blockFrame->CreateAddBackgroundPicBox();
	backPicBox->SetColor(PX2_UISM.Color_ViewBackground);

	return blockFrame;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *EU_Manager::CreateUIAuiBlockFrame2(UIAuiBlockFrame *parent,
	UILayoutPosType pos, UIAuiBlockFrame *&outCenter, const Sizef &size)
{
	std::vector<MovablePtr> children = parent->GetChildren();
	parent->DestoryBackgroundPicBox();
	parent->DetachAllChildren();

	UIAuiBlockFrame *blockFrame = parent->CreateAddPosFrame(pos, size);
	UIAuiBlockFrame *blockFrameCenter = parent->CreateAddPosFrame(UILPT_CENTER,
		size);
	outCenter = blockFrameCenter;
	blockFrame->SetBrotherFrame(blockFrameCenter);
	blockFrameCenter->SetBrotherFrame(blockFrame);

	UICanvas *canvas = new0 UICanvas();
	blockFrame->AttachChild(canvas);
	canvas->SetBeforeDrawClear(true, true, true);
	canvas->SetAnchorHor(0.0f, 1.0f);
	canvas->SetAnchorVer(0.0f, 1.0f);
	Float3 cc = PX2_UISM.Color_ViewBackground;
	canvas->SetClearColor(Float4(cc[0], cc[1], cc[2], cc[3]));

	for (int i = 0; i < (int)children.size(); i++)
	{
		if (children[i] && "BackPicBox"!=children[i]->GetName() && 
			"MaskPicBox" != children[i]->GetName())
			blockFrameCenter->AttachChild(children[i]);
	}

	blockFrame->SetName(PX2_UIAUIM.GenName("UIAuiBlockFrame"));

	UITabFrame *tabFrame = new0 UITabFrame();
	canvas->AttachChild(tabFrame);
	tabFrame->SetName("UITabFrame_" + blockFrame->GetName());
	tabFrame->SetUICallback(_UITabFrameCallback);

	PX2_UIAUIM.AddTabFrame(tabFrame->GetName(), tabFrame);

	return blockFrame;
}
//----------------------------------------------------------------------------
void EU_Manager::Visit(Object *obj, int info)
{
	const std::string &name = obj->GetName();
	UIButton *but = DynamicCast<UIButton>(obj);
	if (but)
	{
		if (UICT_PRESSED == info)
		{

		}
		else if (UICT_RELEASED == info)
		{
			if ("ButSimu" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(1);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("ButPlay" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(2);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
			else if ("ButPlayInWindow" == name)
			{
				Event *ent = EditEventSpace::CreateEventX(EditEventSpace::N_Simu);
				ent->SetData<int>(3);
				PX2_EW.BroadcastingLocalEvent(ent);
			}
		}
	}
}
//----------------------------------------------------------------------------