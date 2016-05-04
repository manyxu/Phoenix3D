// PX2LP_Manager.cpp

#include "PX2LP_Manager.hpp"
#include "PX2EventWorld.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Project.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2EngineLoop.hpp"
#include "PX2NetInitTerm.hpp"
#include "PX2AweUIFrame.hpp"
#include <Awesomium/WebCore.h>
using namespace PX2;
using namespace Awesomium;

//----------------------------------------------------------------------------
LP_Manager::LP_Manager()
{
}
//----------------------------------------------------------------------------
LP_Manager::~LP_Manager()
{
}
//----------------------------------------------------------------------------
void TabFrameUICallback (UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	UITabFrame *tabFrame = DynamicCast<UITabFrame>(frame);
	if (tabFrame && "TheTabFrame" == name)
	{
		if (UICT_TABFRAME_SETACTIVE == type)
		{
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::Initlize()
{
	PX2_EW.ComeIn(this);

	PX2_SC_LUA->CallFile("DataLauncher/scripts/start.lua");

	RenderWindow *rw = PX2_GR.GetMainWindow();
	UICanvas *mainCanvas = (UICanvas*)(rw->GetCanvas("Main"));

	UIFrame *uiMainFrame = new0 UIFrame();
	mainCanvas->AttachChild(uiMainFrame);
	UIPicBox *picBox = uiMainFrame->CreateAddBackgroundPicBox();
	uiMainFrame->SetAnchorHor(0.0f, 1.0f);
	uiMainFrame->SetAnchorVer(0.0f, 1.0f);
	picBox->SetColor(Float3::BLACK);

	// top
	float topHeight = 64.0f;
	UIFrame *frameTop = new0 UIFrame();
	uiMainFrame->AttachChild(frameTop);
	frameTop->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *topBackPic = frameTop->CreateAddBackgroundPicBox();
	topBackPic->SetColor(Float3::WHITE);
	frameTop->SetAnchorHor(0.0f, 1.0f);
	frameTop->SetAnchorVer(1.0f, 1.0f);
	frameTop->SetSize(0.0f, topHeight);
	frameTop->SetAnchorParamVer(-topHeight/2.0f, 0.0f);

	UIFrame *headFrame = CreateHeadFrame();
	frameTop->AttachChild(headFrame);


	mTableFrame = new0 UITabFrame();
	mTableFrame->SetName("TheTabFrame");
	mTableFrame->SetUICallback(TabFrameUICallback);
	uiMainFrame->AttachChild(mTableFrame);
	mTableFrame->SetTabBarHeight(40.0f);
	mTableFrame->SetTabHeight(36.0f);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *tabBackPic = mTableFrame->CreateAddBackgroundPicBox();
	tabBackPic->SetColor(Float3::WHITE);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);
	mTableFrame->SetAnchorParamVer(0.0f, -topHeight);

	// Awesomium
	WebCore* web_core = WebCore::Initialize(WebConfig());

	UIFrame *frameHub = AddTabFrame("PhoenixHub", PX2_LMVAL("PhoenixHub"));
	UIFrame *hubFrame = CreateHubFrame();
	frameHub->AttachChild(hubFrame);

	UIFrame *frameEngine = AddTabFrame("Engine", PX2_LMVAL("Engine"));
	UIFrame *engineFrame = CreateEngineFrame();
	frameEngine->AttachChild(engineFrame);

	UIFrame *frameMarket = AddTabFrame("Market", PX2_LMVAL("Market"));
	AweUIFrame *aweMarket = new0 AweUIFrame();
	frameMarket->AttachChild(aweMarket);
	aweMarket->SetAnchorHor(0.0f, 1.0f);
	aweMarket->SetAnchorVer(0.0f, 1.0f);
	aweMarket->OpenURL("http://www.baidu.com");

	UIFrame *frameYouMake = AddTabFrame("YouMake", PX2_LMVAL("YouMake"));
	AweUIFrame *aweYouMake = new0 AweUIFrame();
	frameYouMake->AttachChild(aweYouMake);
	aweYouMake->SetAnchorHor(0.0f, 1.0f);
	aweYouMake->SetAnchorVer(0.0f, 1.0f);
	aweYouMake->OpenURL("http://sg.zuiyouxi.com/");

	//AddTabFrame("DogGait", PX2_LMVAL("DogGait"));
	//AddTabFrame("Toys", PX2_LMVAL("Toys"));
	mTableFrame->SetActiveTab("PhoenixHub");

}
//----------------------------------------------------------------------------
void LP_Manager::Terminate()
{
	mTableFrame = 0;

	PX2_UIAUIM.Clear();

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->RemoveAllCanvas();

	PX2_EW.GoOut(this);

	TerminateNetwork();
}
//----------------------------------------------------------------------------
void LP_Manager::Update(double appSeconds, double elapsedSeconds)
{
}
//----------------------------------------------------------------------------
void LP_Manager::DoExecute(Event *event)
{
}
//----------------------------------------------------------------------------
void LP_Manager::Visit(Object *obj, int info)
{	
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateHeadFrame()
{
	UIFrame *headFrame = new0 UIFrame();
	headFrame->SetPivot(0.0f, 0.5f);
	headFrame->SetAnchorHor(0.0f, 0.0f);
	headFrame->SetAnchorVer(0.0f, 1.0f);
	headFrame->SetSize(200.0f, 0.0f);

	UIButton *headBut = new0 UIButton();
	headFrame->AttachChild(headBut);
	headBut->SetAnchorHor(0.0f, 0.0f);
	headBut->SetAnchorVer(0.5f, 0.5f);
	headBut->SetSize(60.0f, 60.0f);
	headBut->SetAnchorParamHor(40.0f, 0.f);
	headBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"DataLauncher/images/icons/h_brid_72.png");
	headBut->SetStateColor(UIButtonBase::BS_NORMAL, Float3::WHITE);
	headBut->SetStateColor(UIButtonBase::BS_HOVERED, Float3::WHITE);
	headBut->SetStateColor(UIButtonBase::BS_PRESSED, Float3::WHITE);
	headBut->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
	headBut->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.0f);
	headBut->SetStateBrightness(UIButtonBase::BS_PRESSED, 1.05f);

	UIFText *nameText = new0 UIFText();
	headBut->AttachChild(nameText);
	nameText->LocalTransform.SetTranslateY(-2.0f);
	nameText->SetAnchorHor(0.5f, 0.5f);
	nameText->SetAnchorVer(0.0f, 0.0f);
	nameText->SetAnchorParamVer(7.0f, 0.0f);
	nameText->GetText()->SetFont("DataLauncher/fonts/msyh.ttc", 24, 24);
	nameText->GetText()->SetFontScale(0.5f);
	nameText->GetText()->SetFontColor(Float3::WHITE);
	nameText->GetText()->SetDrawStyle(FD_BORDER);
	nameText->GetText()->SetBorderShadowAlpha(0.5f);
	nameText->GetText()->SetText(PX2_LMVAL("mrtime"));

	return headFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::AddTabFrame(const std::string &name, 
	const std::string &title)
{
	UIFrame *uiFrame = new0 UIFrame();
	uiFrame->SetAnchorHor(0.0f, 1.0f);
	uiFrame->SetAnchorVer(0.0f, 1.0f);
	uiFrame->SetActivateSelfCtrled(true);

	UIFPicBox *fpicBox = new0 UIFPicBox();
	uiFrame->AttachChild(fpicBox);
	fpicBox->LocalTransform.SetTranslateY(-10.0f);
	fpicBox->SetAnchorHor(0.0f, 1.0f);
	fpicBox->SetAnchorVer(1.0f, 1.0f);
	fpicBox->SetPivot(1.0f, 1.0f);
	fpicBox->SetSize(0.0f, 3.0f);
	UIPicBox *picBox = fpicBox->GetUIPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::YELLOW);
	
	mTableFrame->AddTab(name, title, uiFrame);
	UIText *text = mTableFrame->GetTabButton(name)->GetText();
	text->SetColorSelfCtrled(true);
	text->SetBrightnessSelfCtrled(true); 
	text->SetFont("DataLauncher/fonts/msyh.ttc", 24, 24);
	text->SetFontScale(0.65f);
	text->SetFontColor(Float3::WHITE);
	text->SetColor(Float3::WHITE);
	text->SetDrawStyle(FD_SHADOW);
	text->SetBorderShadowAlpha(0.8f);

	UIButton *tabBut = mTableFrame->GetTabButton(name);

	Float3 color = Float3::MakeColor(237, 28, 36);
	if ("PhoenixHub" == name)
	{
		color = Float3::MakeColor(237, 28, 36);
	}
	else if ("Engine" == name)
	{
		color = Float3::MakeColor(255, 127, 39);
	}
	else if ("YouMake" == name)
	{
		color = Float3::MakeColor(24, 177, 76);
	}

	picBox->SetColor(color);
	tabBut->SetStateColor(UIButtonBase::BS_NORMAL, color);
	tabBut->SetStateColor(UIButtonBase::BS_HOVERED, color);
	tabBut->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	tabBut->SetStateColor(UIButtonBase::BS_PRESSED, color);
	tabBut->SetStateBrightness(UIButtonBase::BS_PRESSED, 1.0f);
	tabBut->SetActivateColor(color);

	return uiFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateHubFrame()
{
	UIFrame *hubFrame = new0 UIFrame();
	hubFrame->LocalTransform.SetTranslateY(-1.0f);
	hubFrame->SetAnchorHor(0.0f, 1.0f);
	hubFrame->SetAnchorVer(0.0f, 1.0f);
	hubFrame->SetAnchorParamVer(50.0f, 0.0f);

	UIFPicBox *fpicBox = new0 UIFPicBox();
	hubFrame->AttachChild(fpicBox);
	fpicBox->GetUIPicBox()->SetTexture("DataLauncher/images/phoenixhub.png");
	fpicBox->SetAnchorHor(0.5f, 0.5f);
	fpicBox->SetAnchorVer(0.5f, 0.5f);
	fpicBox->SetSize(775.0f, 458.0f);

	UIFText *text0 = new0 UIFText();
	hubFrame->AttachChild(text0);
	text0->GetText()->SetFont("DataLauncher/fonts/msyh.ttc", 24, 24);
	text0->SetSize(400.0f, 50.0f);
	text0->SetAnchorHor(0.5f, 0.5f);
	text0->SetAnchorVer(0.5f, 0.5f);
	text0->SetAnchorParamVer(-270.0f, 0.0f);
	text0->GetText()->SetText(PX2_LMVAL("sryy"));

	UIFText *text1 = new0 UIFText();
	hubFrame->AttachChild(text1);
	text1->GetText()->SetFont("DataLauncher/fonts/msyh.ttc", 24, 24);
	text1->GetText()->SetFontScale(0.6f);
	text1->GetText()->SetFontStyle(FES_UNDERLINE);
	text1->SetSize(200.0f, 50.0f);
	text1->SetAnchorHor(0.5f, 0.5f);
	text1->SetAnchorVer(0.5f, 0.5f);
	text1->SetAnchorParamVer(-300.0f, 0.0f);
	text1->GetText()->SetText(PX2_LMVAL("webaddress"));

	return hubFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateEngineFrame()
{
	UIFrame *engineFrame = new0 UIFrame();
	engineFrame->LocalTransform.SetTranslateY(-1.0f);
	engineFrame->SetAnchorHor(0.0f, 1.0f);
	engineFrame->SetAnchorVer(0.0f, 1.0f);

	float leftWidth = 200.0f;

	float engineButWidth = leftWidth - 10.0f;
	float engineButHeight = 50.0f;

	UIFrame *leftFrame = new0 UIFrame();
	engineFrame->AttachChild(leftFrame);
	leftFrame->SetAnchorHor(0.0f, 0.0f);
	leftFrame->SetAnchorVer(0.0f, 1.0f);
	leftFrame->SetAnchorParamHor(leftWidth / 2.0f, 0.0f);
	leftFrame->SetSize(leftWidth, 0.0f);
	UIPicBox *picBox = leftFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(Float3::MakeColor(64, 64, 64));

	float height = -80.0f;
	// engine
	UIButton *butEngine = new0 UIButton();
	leftFrame->AttachChild(butEngine);
	butEngine->SetAnchorHor(0.5f, 0.5f);
	butEngine->SetAnchorVer(1.0f, 1.0f);
	butEngine->SetAnchorParamVer(height, 0.0f);
	butEngine->LocalTransform.SetTranslateY(-1.0f);
	butEngine->SetSize(engineButWidth, engineButHeight);

	// news
	height -= 100.0f;
	UIButton *butNews = new0 UIButton();
	butNews->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(80, 80, 80));
	butNews->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(100, 100, 100));
	butNews->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(60, 60, 60));
	leftFrame->AttachChild(butNews);
	butNews->SetAnchorHor(0.5f, 0.5f);
	butNews->SetAnchorVer(1.0f, 1.0f);
	butNews->SetAnchorParamVer(height, 0.0f);
	butNews->LocalTransform.SetTranslateY(-1.0f);
	butNews->SetSize(leftWidth, engineButHeight);

	// learn
	height -= 55.0f;
	UIButton *butLearn = new0 UIButton();
	leftFrame->AttachChild(butLearn);
	butLearn->SetAnchorHor(0.5f, 0.5f);
	butLearn->SetAnchorVer(1.0f, 1.0f);
	butLearn->SetAnchorParamVer(height, 0.0f);
	butLearn->LocalTransform.SetTranslateY(-1.0f);
	butLearn->SetSize(leftWidth, engineButHeight);

	// res
	height -= 55.0f;
	UIButton *butRes = new0 UIButton();
	leftFrame->AttachChild(butRes);
	butRes->SetAnchorHor(0.5f, 0.5f);
	butRes->SetAnchorVer(1.0f, 1.0f);
	butRes->SetAnchorParamVer(height, 0.0f);
	butRes->LocalTransform.SetTranslateY(-1.0f);
	butRes->SetSize(leftWidth, engineButHeight);

	UIFrame *rightFrame = new0 UIFrame();
	engineFrame->AttachChild(rightFrame);
	rightFrame->SetAnchorHor(0.0f, 1.0f);
	rightFrame->SetAnchorVer(0.0f, 1.0f);
	rightFrame->SetAnchorParamHor(leftWidth, 0.0f);

	AweUIFrame *aweFrame = new0 AweUIFrame();
	rightFrame->AttachChild(aweFrame);
	aweFrame->SetAnchorHor(0.0f, 1.0f);
	aweFrame->SetAnchorVer(0.0f, 1.0f);
	aweFrame->OpenURL("http://www.google.com/");

	return engineFrame;
}
//----------------------------------------------------------------------------