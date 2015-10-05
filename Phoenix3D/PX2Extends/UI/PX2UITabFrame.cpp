// PX2UITabFrame.cpp

#include "PX2UITabFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UITabFrame);
PX2_IMPLEMENT_STREAM(UITabFrame);
PX2_IMPLEMENT_FACTORY(UITabFrame);

//----------------------------------------------------------------------------
UITabFrame::UITabFrame() :
mTabWidth(100.0f),
mAuiBlockFrame(0),
mIsTabsNeedReCal(true)
{
	float titleBarHeight = 20.0f;

	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetPvoit(0.5f, 0.5f);

	mFrame_TitleBar = new0 UIFrame();
	AttachChild(mFrame_TitleBar);
	mFrame_TitleBar->SetName("TitleBar");
	mFrame_TitleBar->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mFrame_TitleBar->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::MakeColor(60, 60, 60));
	mFrame_TitleBar->SetSize(0.0f, titleBarHeight);
	mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_TitleBar->SetAnchorVer(Float2(1.0f, 1.0f));
	mFrame_TitleBar->SetPvoit(0.5f, 1.0f);

	mFrame_Content = new0 UIFrame();
	AttachChild(mFrame_Content);
	mFrame_Content->SetName("Content");
	mFrame_Content->SetAnchorHor(0.0f, 1.0f);
	mFrame_Content->SetAnchorVer(0.0f, 1.0f);
	mFrame_Content->SetAnchorParamVer(0.0f, titleBarHeight);
	UIPicBox *contPicBox = mFrame_Content->CreateAddBackgroundPicBox();
	contPicBox->SetTexture("Data/engine/white.png");
	contPicBox->SetColor(Float3::MakeColor(37, 37, 38));
}
//----------------------------------------------------------------------------
UITabFrame::~UITabFrame()
{
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabWidth(float width)
{
	mTabWidth = width;

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::TabCallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	SetActiveTab(name);
}
//----------------------------------------------------------------------------
void UITabFrame::AddTab(const std::string &name, UIFrame *tabFrame)
{
	if (!tabFrame) return;

	std::map<std::string, UIFramePtr>::iterator it = mTabFrames.find(name);
	if (it == mTabFrames.end())
	{
		UIAuiManager::GetSingleton().AddTabItemFrame(name, tabFrame);

		tabFrame->Show(false);

		mTabFrames[name] = tabFrame;
		mFrame_Content->AttachChild(tabFrame);

		UIButton *tabBut = new0 UIButton();	
		mFrame_TitleBar->AttachChild(tabBut);
		mTabTitles[name] = tabBut;
		tabBut->LocalTransform.SetTranslateY(-5.0f);
		tabBut->SetName(name);
		tabBut->SetMemUIUICallback(this, (UIFrame::MemUICallback)(&UITabFrame::TabCallback));

		UIText *text = tabBut->CreateAddText();
		text->SetText(name);
		text->SetFontScale(0.6f);

		UIPicBox *picBoxNormal = tabBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
		picBoxNormal->SetTexture("Data/engine/white.png");
		picBoxNormal->SetColor(Float3::MakeColor(120, 120, 120));

		UIPicBox *picBoxPressed = tabBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED);
		picBoxPressed->SetTexture("Data/engine/white.png");
		picBoxPressed->SetColor(Float3::MakeColor(180, 180, 180));

		mIsTabsNeedReCal = true;
	}
}
//----------------------------------------------------------------------------
bool UITabFrame::IsHasTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabFrames.find(name);

	return (it!=mTabFrames.end());
}
//----------------------------------------------------------------------------
UIFrame *UITabFrame::GetTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabFrames.find(name);
	if (it != mTabFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UITabFrame::RemoveTab(const std::string &name)
{
	// cot
	std::map<std::string, UIFramePtr>::iterator it = mTabFrames.find(name);
	if (it != mTabFrames.end())
	{
		mFrame_Content->DetachChild(it->second);
		mTabFrames.erase(it);
	}

	// tab but
	std::map<std::string, UIFramePtr>::iterator it1 = mTabTitles.find(name);
	if (it1 != mTabTitles.end())
	{
		mFrame_TitleBar->DetachChild(it1->second);
		mTabTitles.erase(it1);
	}

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetActiveTab(const std::string &name)
{
	if (mActiveTabFrame)
		mActiveTabFrame->Show(false);

	UIFrame *tabFrame = GetTab(name);
	if (tabFrame)
	{
		mActiveTabFrame = tabFrame;
	}

	if (mActiveTabFrame)
		mActiveTabFrame->Show(true);
}
//----------------------------------------------------------------------------
void UITabFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsTabsNeedReCal)
		_CalTabs();

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UITabFrame::_CalTabs()
{
	int index = 0;

	std::map<std::string, UIFramePtr>::iterator it = mTabTitles.begin();
	for (; it != mTabTitles.end(); it++)
	{
		UIFrame *tabTitle = it->second;
		tabTitle->SetSize(mTabWidth, mFrame_TitleBar->GetHeight());
		tabTitle->SetAnchorHor(0.0f, 0.0f);
		tabTitle->SetAnchorParamHor(mTabWidth*0.5f + mTabWidth*index, 0.0f);
		tabTitle->SetAnchorVer(0.0f, 1.0f);

		index++;
	}

	mIsTabsNeedReCal = false;
}
//----------------------------------------------------------------------------
void UITabFrame::SetAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame)
{
	mAuiBlockFrame = auiBlockFrame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UITabFrame::UITabFrame(LoadConstructor value) :
UIFrame(value),
mAuiBlockFrame(0),
mIsTabsNeedReCal(true)
{
}
//----------------------------------------------------------------------------
void UITabFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UITabFrame, source);
}
//----------------------------------------------------------------------------
void UITabFrame::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UITabFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UITabFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UITabFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UITabFrame, target);
}
//----------------------------------------------------------------------------
int UITabFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------