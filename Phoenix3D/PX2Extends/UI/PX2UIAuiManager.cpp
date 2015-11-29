// PX2UIAuiManager.cpp

#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int UIAuiManager::msNameID = 0;
//----------------------------------------------------------------------------
UIAuiManager::UIAuiManager()
{
}
//----------------------------------------------------------------------------
UIAuiManager::~UIAuiManager()
{
}
//----------------------------------------------------------------------------
std::string UIAuiManager::GenName(const std::string &tag)
{
	msNameID++;

	return tag + StringHelp::IntToString(msNameID);
}
//----------------------------------------------------------------------------
void UIAuiManager::SetAuiFrame(UIAuiFrame *frame)
{
	mUIAuiFrame = frame;
}
//----------------------------------------------------------------------------
void UIAuiManager::AddUIAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame)
{
	mUIAuiBlockFrames.push_back(auiBlockFrame);
}
//----------------------------------------------------------------------------
void UIAuiManager::RemoveUIAuiBlockFrame(UIAuiBlockFrame *auiBlockframe)
{
	std::vector<UIAuiBlockFramePtr>::iterator it = mUIAuiBlockFrames.begin();
	for (; it != mUIAuiBlockFrames.end(); it++)
	{
		if (auiBlockframe == *it)
		{
			mUIAuiBlockFrames.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void UIAuiManager::AddTabFrame(const std::string &name, UITabFrame *tab)
{
	std::map<std::string, UITabFramePtr>::iterator it = 
		mAuiTabFrames.find(name);
	if (it == mAuiTabFrames.end())
	{
		mAuiTabFrames[name] = tab;
	}
	else
	{
		assertion(false, "tab %s already exist.", name.c_str());
	}
}
//----------------------------------------------------------------------------
void UIAuiManager::RemoveTabFrame(const std::string &name)
{
	mAuiTabFrames.erase(name);
}
//----------------------------------------------------------------------------
void UIAuiManager::RemoveTabFrame(UITabFrame *tabFrame)
{
	RemoveTabFrame(tabFrame->GetName());
}
//----------------------------------------------------------------------------
UITabFrame *UIAuiManager::GetTabFrame(const std::string &name)
{
	std::map<std::string, UITabFramePtr>::iterator it =
		mAuiTabFrames.find(name);
	if (it != mAuiTabFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIAuiManager::AddTabContentFrame(UIButton *tabBut, UIFrame *frame)
{
	mAuiContentFrames[tabBut] = frame;
}
//----------------------------------------------------------------------------
void UIAuiManager::SetActiveTableFrame(UIButton *tabBut, UIFrame *tableFrame)
{
	if (mActiveTableBut)
	{
		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(
			PX2_UISM.Color_AuiButTab_Normal);

		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(
			PX2_UISM.Color_AuiButTab_Horvered);

		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(
			PX2_UISM.Color_AuiButTab_Pressed);

		mActiveTableContentFrame = 0;
	}

	mActiveTableBut = tabBut;
	mActiveTableContentFrame = tableFrame;

	if (mActiveTableBut)
	{
		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetColor(
			PX2_UISM.Color_AuiButTab_Active);

		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetColor(
			PX2_UISM.Color_AuiButTab_Active);

		mActiveTableBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetColor(
			PX2_UISM.Color_AuiButTab_Active);
	}
}
//----------------------------------------------------------------------------