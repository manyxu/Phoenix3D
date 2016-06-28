// PX2UIAuiManager.cpp

#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2StringHelp.hpp"
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
void UIAuiManager::Clear()
{
	mUIAuiFrames.clear();

	mAuiTabFrames.clear();

	mAuiContentFrames.clear();

	mActiveTableBut = 0;
	mActiveTableContentFrame = 0;
}
//----------------------------------------------------------------------------
std::string UIAuiManager::GenName(const std::string &tag)
{
	msNameID++;

	return tag + StringHelp::IntToString(msNameID);
}
//----------------------------------------------------------------------------
void UIAuiManager::AddAuiFrame(RenderWindow *rw, UIAuiFrame *frame)
{
	mUIAuiFrames[rw] = frame;
}
//----------------------------------------------------------------------------
UIAuiFrame *UIAuiManager::GetAuiFrame(RenderWindow *rw)
{
	std::map<RenderWindow *, UIAuiFramePtr>::iterator it = 
		mUIAuiFrames.find(rw);
	if (it != mUIAuiFrames.end())
	{
		return it->second;
	}

	return 0;
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
	const std::string &tabName = tabBut->GetName();
	std::map<std::string, UIButtonPtr>::iterator it = mAuiTabButs.find(tabName);
	if (it == mAuiTabButs.end())
	{
		mAuiTabButs[tabName] = tabBut;
		mAuiContentFrames[tabBut] = frame;
	}
	else
	{
		assertion(false, "tabBut %s already exist.", tabName.c_str());
	}
}
//----------------------------------------------------------------------------
void UIAuiManager::SetActiveTableFrame(const std::string &tabName)
{
	std::map<std::string, UIButtonPtr>::iterator it = mAuiTabButs.find(tabName);
	if (it != mAuiTabButs.end())
	{
		UIButton *tabBut = it->second;
		UIFrame *uiContentFrame = mAuiContentFrames[tabBut];
		UITabFrame *tabFrame = DynamicCast<UITabFrame>(
			uiContentFrame->GetParent()->GetParent());

		tabFrame->_SetActiveTab(tabBut->GetName());

		if (mActiveTableBut)
		{
			mActiveTableBut->SetActivate(false);
			mActiveTableBut = 0;
		}

		if (mActiveTableContentFrame)
		{
			mActiveTableContentFrame->SetActivate(false);
			mActiveTableContentFrame = 0;
		}

		mActiveTableBut = tabBut;
		mActiveTableContentFrame = uiContentFrame;

		if (mActiveTableBut)
			mActiveTableBut->SetActivate(true);

		if (mActiveTableContentFrame)
			mActiveTableContentFrame->SetActivate(true);
	}
}
//----------------------------------------------------------------------------
void UIAuiManager::SetCaptureBlockFrame(UIAuiBlockFrame *blockFrame)
{
	mCaptureBlockFrame = blockFrame;
}
//----------------------------------------------------------------------------