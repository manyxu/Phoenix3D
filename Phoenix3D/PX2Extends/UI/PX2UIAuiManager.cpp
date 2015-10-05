// PX2UIAuiManager.cpp

#include "PX2UIAuiManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UIAuiManager::UIAuiManager()
{
}
//----------------------------------------------------------------------------
UIAuiManager::~UIAuiManager()
{
}
//----------------------------------------------------------------------------
void UIAuiManager::SetAuiFrame(UIAuiFrame *frame)
{
	mUIAuiFrame = frame;
}
//----------------------------------------------------------------------------
void UIAuiManager::SetActiveTableName(const std::string &tabName)
{
	mActiveTableName = tabName;
}
//----------------------------------------------------------------------------
void UIAuiManager::AddTabItemFrame(const std::string &name, UIFrame *frame)
{
	mAuiItemFrames[name] = frame;
}
//----------------------------------------------------------------------------