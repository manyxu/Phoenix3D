// EU_ProjectFrame.cpp

#include "PX2EU_ProjectFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIFPicBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ProjectFrame);
PX2_IMPLEMENT_STREAM(EU_ProjectFrame);
PX2_IMPLEMENT_FACTORY(EU_ProjectFrame);

//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame()
{
	float heightProj = 50.0f;
	mProjFrame = new0 UIFrame();
	AttachChild(mProjFrame);
	mProjFrame->LocalTransform.SetTranslateY(-1.0f);
	mProjFrame->SetAnchorHor(0.0f, 1.0f);
	mProjFrame->SetAnchorVer(1.0f, 1.0f);
	mProjFrame->SetAnchorParamVer(0.0f, 0.0f);
	mProjFrame->SetSize(0.0f, heightProj);
	mProjFrame->SetPivot(0.5f, 1.0f);
	UIPicBox *picBox = mProjFrame->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(PX2_UISM.Color_Splitter);

	float butSize = 36.0f;

	UIFPicBox *backPicBox = new0 UIFPicBox();
	mProjFrame->AttachChild(backPicBox);
	backPicBox->SetAnchorHor(0.0f, 1.0f);
	backPicBox->SetAnchorVer(0.0f, 1.0f);
	backPicBox->SetAnchorParamHor(30.0f, 30.0f);
	backPicBox->SetAnchorParamVer(10.0f, 10.0f);
	backPicBox->GetUIPicBox()->SetTexture("Data/engine/white.png");
	backPicBox->GetUIPicBox()->SetColor(Float3::MakeColor(50, 50, 50));

	UIButton *simuBut = new0 UIButton();
	mProjFrame->AttachChild(simuBut);
	simuBut->LocalTransform.SetTranslateY(-1.0f);
	simuBut->SetSize(butSize, butSize);
	simuBut->SetAnchorHor(0.5f, 0.5f);
	simuBut->SetAnchorVer(0.5f, 0.5f);
	simuBut->SetAnchorParamHor(-50.0f, 0.0f);
	simuBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("DataNIRVANA2/images/icons/project/simulate_40.png");
	simuBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexture("DataNIRVANA2/images/icons/project/simulate_40.png");
	simuBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetBrightness(1.2f);
	simuBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture("DataNIRVANA2/images/icons/project/simulate_40.png");

	UIButton *playBut = new0 UIButton();
	mProjFrame->AttachChild(playBut);
	playBut->LocalTransform.SetTranslateY(-1.0f);
	playBut->SetSize(butSize*0.95f, butSize*0.95f);
	playBut->SetAnchorHor(0.5f, 0.5f);
	playBut->SetAnchorVer(0.5f, 0.5f);
	playBut->SetAnchorParamHor(0.0f, 0.0f);
	playBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("DataNIRVANA2/images/icons/project/play_40.png");
	playBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexture("DataNIRVANA2/images/icons/project/play_40.png");
	playBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetBrightness(1.2f);
	playBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture("DataNIRVANA2/images/icons/project/play_40.png");

	UIButton *playInWindowBut = new0 UIButton();
	mProjFrame->AttachChild(playInWindowBut);
	playInWindowBut->LocalTransform.SetTranslateY(-1.0f);
	playInWindowBut->SetSize(butSize*0.85f, butSize*0.85f);
	playInWindowBut->SetAnchorHor(0.5f, 0.5f);
	playInWindowBut->SetAnchorVer(0.5f, 0.5f);
	playInWindowBut->SetAnchorParamHor(50.0f, 0.0f);
	playInWindowBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("DataNIRVANA2/images/icons/project/playinwindow_40.png");
	playInWindowBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexture("DataNIRVANA2/images/icons/project/playinwindow_40.png");
	playInWindowBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetBrightness(1.2f);
	playInWindowBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture("DataNIRVANA2/images/icons/project/playinwindow_40.png");

	mTableFrame = new0 UITabFrame();
	AttachChild(mTableFrame);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	mTableFrame->SetTabWidth(80.0f);
	mTableFrame->SetLayoutPos(UITabFrame::LPT_TOP);
	mTableFrame->SetSkinAui(false);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);
	mTableFrame->SetAnchorParamVer(0.0f, heightProj);

	mTreeProject = new0 EU_ProjectTree(EU_ProjectTree::PTT_PROJECT);
	mTreeProject->LocalTransform.SetTranslateY(-1.0f);
	mTreeProject->SetAnchorHor(0.0f, 1.0f);
	mTreeProject->SetAnchorVer(0.0f, 1.0f);

	mTreeScene = new0 EU_ProjectTree(EU_ProjectTree::PTT_SCENE);
	mTreeScene->LocalTransform.SetTranslateY(-1.0f);
	mTreeScene->SetAnchorHor(0.0f, 1.0f);
	mTreeScene->SetAnchorVer(0.0f, 1.0f);

	mTreeUI = new0 EU_ProjectTree(EU_ProjectTree::PTT_UI);
	mTreeUI->LocalTransform.SetTranslateY(-1.0f);
	mTreeUI->SetAnchorHor(0.0f, 1.0f);
	mTreeUI->SetAnchorVer(0.0f, 1.0f);

	mTableFrame->AddTab("Project", mTreeProject);
	mTableFrame->AddTab("Scene", mTreeScene);
	mTableFrame->AddTab("UI", mTreeUI);
	mTableFrame->SetActiveTab("Project");
}
//----------------------------------------------------------------------------
EU_ProjectFrame::~EU_ProjectFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ProjectFrame, source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ProjectFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ProjectFrame, target);
}
//----------------------------------------------------------------------------
int EU_ProjectFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------