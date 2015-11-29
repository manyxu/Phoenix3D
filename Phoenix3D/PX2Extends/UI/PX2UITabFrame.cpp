// PX2UITabFrame.cpp

#include "PX2UITabFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UITabFrame);
PX2_IMPLEMENT_STREAM(UITabFrame);
PX2_IMPLEMENT_FACTORY(UITabFrame);

//----------------------------------------------------------------------------
UITabFrame::UITabFrame() :
mLayoutPosType(LPT_TOP),
mTabWidth(100.0f),
mTabHeight(20.0f),
mAuiBlockFrame(0),
mIsTabsNeedReCal(true),
mIsSkinAui(true)
{
	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetPivot(0.5f, 0.5f);

	mFrame_TitleBar = new0 UIFrame();
	AttachChild(mFrame_TitleBar);
	mFrame_TitleBar->SetName("TitleBar");
	mFrame_TitleBar->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mFrame_TitleBar->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::MakeColor(60, 60, 60));
	mFrame_TitleBar->SetSize(0.0f, mTabHeight);
	mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_TitleBar->SetAnchorVer(Float2(1.0f, 1.0f));
	mFrame_TitleBar->SetPivot(0.5f, 1.0f);

	mFrame_Content = new0 UIFrame();
	AttachChild(mFrame_Content);
	mFrame_Content->SetName("Content");
	mFrame_Content->SetAnchorHor(0.0f, 1.0f);
	mFrame_Content->SetAnchorVer(0.0f, 1.0f);
	mFrame_Content->SetAnchorParamVer(0.0f, mTabHeight);
}
//----------------------------------------------------------------------------
UITabFrame::~UITabFrame()
{
}
//----------------------------------------------------------------------------
void UITabFrame::SetLayoutPos(LayoutPosType posType)
{
	mLayoutPosType = posType;

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetSkinAui(bool isAui)
{
	mIsSkinAui = isAui;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabWidth(float width)
{
	mTabWidth = width;

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabHeight(float height)
{
	mTabHeight = height;
	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::TabCallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	SetActiveTab(name);
}
//----------------------------------------------------------------------------
void UITabFrame::AddTab(const std::string &name, UIFrame *tabContentFrame)
{
	if (!tabContentFrame) return;

	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it == mTabContentFrames.end())
	{
		tabContentFrame->Show(false);

		mTabContentFrames[name] = tabContentFrame;
		mFrame_Content->AttachChild(tabContentFrame);

		UIButton *tabBut = new0 UIButton();	
		mFrame_TitleBar->AttachChild(tabBut);
		mTabButs.push_back(tabBut);
		tabBut->LocalTransform.SetTranslateY(-5.0f);
		tabBut->SetName(name);
		tabBut->SetMemUICallback(this, (UIFrame::MemUICallback)(&UITabFrame::TabCallback));

		UIText *text = tabBut->CreateAddText();
		text->SetText(name);
		text->SetFontScale(0.6f);

		UIPicBox *picBoxNormal = tabBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
		picBoxNormal->SetTexture("Data/engine/white.png");
		if (mIsSkinAui)
			picBoxNormal->SetColor(PX2_UISM.Color_AuiButTab_Normal);
		else
			picBoxNormal->SetColor(PX2_UISM.Color_ButTab_Normal);

		UIPicBox *picBoxHovered = tabBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED);
		picBoxHovered->SetTexture("Data/engine/white.png");
		if (mIsSkinAui)
			picBoxHovered->SetColor(PX2_UISM.Color_AuiButTab_Horvered);
		else
			picBoxHovered->SetColor(PX2_UISM.Color_ButTab_Horvered);

		UIPicBox *picBoxPressed = tabBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED);
		picBoxPressed->SetTexture("Data/engine/white.png");
		if (mIsSkinAui)
			picBoxPressed->SetColor(PX2_UISM.Color_AuiButTab_Pressed);
		else
			picBoxPressed->SetColor(PX2_UISM.Color_ButTab_Pressed);

		UIAuiManager::GetSingleton().AddTabContentFrame(tabBut, tabContentFrame);

		mIsTabsNeedReCal = true;
	}
}
//----------------------------------------------------------------------------
bool UITabFrame::IsHasTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);

	return (it!=mTabContentFrames.end());
}
//----------------------------------------------------------------------------
UIFrame *UITabFrame::GetTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it != mTabContentFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UITabFrame::RemoveTab(const std::string &name)
{
	// cot
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it != mTabContentFrames.end())
	{
		mFrame_Content->DetachChild(it->second);
		mTabContentFrames.erase(it);
	}

	// tab but
	std::vector<UIButtonPtr>::iterator it1 = mTabButs.begin();
	for (; it1 != mTabButs.end();)
	{
		if ((*it1)->GetName() == name)
		{
			it1 = mTabButs.erase(it1);
		}
		else
		{
			it1++;
		}
	}

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
UIButton *UITabFrame::GetTabButton(const std::string &name)
{
	for (int i = 0; i < (int)mTabButs.size(); i++)
	{
		if (mTabButs[i]->GetName() == name)
			return mTabButs[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
void UITabFrame::SetActiveTab(const std::string &name)
{
	// tabFrame
	if (mActiveTabFrame)
		mActiveTabFrame->Show(false);

	UIFrame *tabFrame = GetTab(name);
	if (tabFrame)
	{
		mActiveTabFrame = tabFrame;
	}

	if (mActiveTabFrame)
		mActiveTabFrame->Show(true);

	// Button
	UIButton *but = GetTabButton(name);
	if (!but)
		return;

	if (IsSkinAui())
	{
		PX2_UIAUIM.SetActiveTableFrame(but, mActiveTabFrame);
	}
	else
	{	
		if (mActiveTabBut)
		{
			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(
				PX2_UISM.Color_ButTab_Normal);

			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(
				PX2_UISM.Color_ButTab_Horvered);

			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(
				PX2_UISM.Color_ButTab_Pressed);
		}

		mActiveTabBut = but;

		if (mActiveTabBut)
		{
			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(
				PX2_UISM.Color_ButTab_Active);

			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(
				PX2_UISM.Color_ButTab_Active);

			mActiveTabBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(
				PX2_UISM.Color_ButTab_Active);
		}
	}
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
	if (LPT_TOP == mLayoutPosType)
	{
		mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
		mFrame_TitleBar->SetAnchorVer(Float2(1.0f, 1.0f));
		mFrame_TitleBar->SetPivot(0.5f, 1.0f);
		mFrame_TitleBar->SetSize(0.0f, mTabHeight);

		mFrame_Content->SetAnchorHor(0.0f, 1.0f);
		mFrame_Content->SetAnchorVer(0.0f, 1.0f);
		mFrame_Content->SetAnchorParamVer(0.0f, mTabHeight);
	}
	else if (LPT_BOTTOM == mLayoutPosType)
	{
		mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
		mFrame_TitleBar->SetAnchorVer(Float2(0.0f, 0.0f));
		mFrame_TitleBar->SetPivot(0.5f, 0.0f);
		mFrame_TitleBar->SetSize(0.0f, mTabHeight);

		mFrame_Content->SetAnchorHor(0.0f, 1.0f);
		mFrame_Content->SetAnchorVer(0.0f, 1.0f);
		mFrame_Content->SetAnchorParamVer(mTabHeight, 0.0f);
	}

	for (int i = 0; i < (int)mTabButs.size(); i++)
	{
		UIFrame *tabTitle = mTabButs[i];
		tabTitle->SetSize(mTabWidth, mTabHeight);
		tabTitle->SetAnchorHor(0.0f, 0.0f);
		tabTitle->SetAnchorParamHor(
			mTabWidth*0.5f + mTabWidth*i + (i*1.0f), 0.0f);
		tabTitle->SetAnchorVer(0.0f, 1.0f);
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
mLayoutPosType(LPT_TOP),
mAuiBlockFrame(0),
mIsTabsNeedReCal(true),
mIsSkinAui(false)
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