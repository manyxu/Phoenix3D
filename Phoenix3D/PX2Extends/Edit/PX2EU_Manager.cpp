// PX2EU_Manager.cpp

#include "PX2EU_Manager.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIView.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2EU_ProjectFrame.hpp"
#include "PX2EU_InspectorFrame.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EU_Manager::EU_Manager()
{
	mMainMenuHeight = 20.0f;
	mStatusBarHeight = 25.0f;
}
//----------------------------------------------------------------------------
EU_Manager::~EU_Manager()
{
}
//----------------------------------------------------------------------------
bool EU_Manager::Initlize()
{
	CrateView_Main();

	return true;
}
//----------------------------------------------------------------------------
bool EU_Manager::Ternamate()
{
	mUIView_Main = 0;
	mFrame_Main = 0;
	mFrame_MainMenu = 0;
	mFrame_StatusBar = 0;

	return true;
}
//----------------------------------------------------------------------------
void EU_Manager::CrateView_Main()
{
	// main
	mUIView_Main = new0 UIView(0);
	mUIView_Main->SetName("EU_Main");
	mUIView_Main->SetRenderer(Renderer::GetDefaultRenderer());
	PX2_GR.AddRenderStep("EU_Main", mUIView_Main);

	// main frame
	CreateFrame_Main();

	// mian menu
	CreateFrame_MainMenu();

	// content
	CreateFrame_Content();

	// status bar
	CreateFrame_StatusBar();

	// project bar
	CreateFrame_Project();

	// inspector bar
	CreateFrame_Inspector();
}
//----------------------------------------------------------------------------
UIView *EU_Manager::GetView_Main()
{
	return mUIView_Main;
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Main()
{
	mFrame_Main = new0 UIFrame();
	mUIView_Main->SetNode(mFrame_Main);
	mFrame_Main->SetSize(500.0f, 300.0f);
	mFrame_Main->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_Main->SetAnchorVer(Float2(0.0f, 1.0f));
	mFrame_Main->SetPvoit(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
UIFrame *EU_Manager::GetFrame_Main()
{
	return mFrame_Main;
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_MainMenu()
{
	mMainMenuHeight = 20.0f;

	mFrame_MainMenu = new0 UIMenu();
	mFrame_Main->AttachChild(mFrame_MainMenu);
	mFrame_MainMenu->SetName("MainMenu");

	UIPicBox *picBox = mFrame_MainMenu->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::MakeColor(240, 240, 240));

	mFrame_MainMenu->SetSize(0.0f, 20.0f);
	mFrame_MainMenu->SetMainItemSize(Sizef(40.0f, mMainMenuHeight));

	mFrame_MainMenu->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_MainMenu->SetAnchorVer(Float2(1.0f, 1.0f));
	mFrame_MainMenu->SetPvoit(0.5f, 1.0f);

	mFrame_MainMenu->AddMainItem("File", "File", "");
	mFrame_MainMenu->AddMainItem("Edit", "Edit", "");
	mFrame_MainMenu->AddMainItem("Windows", "Winds", "");
	mFrame_MainMenu->AddMainItem("Tools", "Tools", "");
	mFrame_MainMenu->AddMainItem("Help", "Help", "");
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Content()
{
	mFrame_Content = new0 UIAuiFrame();
	mFrame_Main->AttachChild(mFrame_Content);
	mFrame_Content->SetName("ContentFrame");

	mFrame_Content->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_Content->SetAnchorVer(Float2(0.0f, 1.0f));
	mFrame_Content->SetAnchorParamVer(Float2(mStatusBarHeight, mMainMenuHeight));
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_StatusBar()
{
	mStatusBarHeight = 25.0f;

	mFrame_StatusBar = new0 UIFrame();
	mFrame_Main->AttachChild(mFrame_StatusBar);
	mFrame_StatusBar->SetName("StatusBar");

	UIPicBox *picBox = mFrame_StatusBar->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::MakeColor(104, 33, 122));

	mFrame_StatusBar->SetSize(0.0f, mStatusBarHeight);

	mFrame_StatusBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_StatusBar->SetAnchorVer(Float2(0.0f, 0.0f));
	mFrame_StatusBar->SetPvoit(0.5f, 0.0f);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Project()
{
	UIAuiBlockFrame *blockFrame = mFrame_Content->AuiBlockDefault
		->CreateAddPosFrame(UILPT_LEFT, Sizef(200.0f, 200.0f));

	UITabFrame *tabFrame = new0 UITabFrame();
	blockFrame->AttachChild(tabFrame);

	EU_ProjectFrame *projFrame = new0 EU_ProjectFrame();
	tabFrame->AddTab("Project", projFrame);
	tabFrame->SetActiveTab("Project");
	projFrame->SetAnchorHor(0.0f, 1.0f);
	projFrame->SetAnchorVer(0.0f, 1.0f);
}
//----------------------------------------------------------------------------
void EU_Manager::CreateFrame_Inspector()
{
	UIAuiBlockFrame *blockFrame = mFrame_Content->AuiBlockDefault
		->CreateAddPosFrame(UILPT_RIGHT, Sizef(200.0f, 200.0f));

	UITabFrame *tabFrame = new0 UITabFrame();
	blockFrame->AttachChild(tabFrame);

	EU_InspectorFrame *inspFrame = new0 EU_InspectorFrame();
	tabFrame->AddTab("Inspector", inspFrame);
	inspFrame->SetAnchorHor(0.0f, 1.0f);
	inspFrame->SetAnchorVer(0.0f, 1.0f);
}
//----------------------------------------------------------------------------