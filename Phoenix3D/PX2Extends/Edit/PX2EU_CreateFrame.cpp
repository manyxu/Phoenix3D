// EU_CreateFrame.cpp

#include "PX2EU_CreateFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2EU_CF_BluePrint.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_CreateFrame);
PX2_IMPLEMENT_STREAM(EU_CreateFrame);
PX2_IMPLEMENT_FACTORY(EU_CreateFrame);

//----------------------------------------------------------------------------
EU_CreateFrame::EU_CreateFrame()
{
	mTableFrame = new0 UITabFrame();
	AttachChild(mTableFrame);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	mTableFrame->SetTabBarHeight(50.0f);
	mTableFrame->SetTabWidth(50.0f);
	mTableFrame->SetTabHeight(50.0f);
	//mTableFrame->SetLayoutPos(UITabFrame::LPT_TOP);
	mTableFrame->SetSkinAui(false);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);

	mTableFrame->AddTab("Objs", "Objs", new0 UIFrame());
	mTableFrame->AddTab("Ters", "Ters", new0 UIFrame());
	mTableFrame->AddTab("Foli", "Foli", new0 UIFrame());

	EU_CF_BluePrint *bpFrame = new0 EU_CF_BluePrint();
	mTableFrame->AddTab("BP", "BP", bpFrame);
	bpFrame->SetAnchorHor(0.0f, 1.0f);
	bpFrame->SetAnchorVer(0.0f, 1.0f);
	mTableFrame->SetActiveTab("Objs");
}
//----------------------------------------------------------------------------
EU_CreateFrame::~EU_CreateFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_CreateFrame::EU_CreateFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_CreateFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_CreateFrame, source);
}
//----------------------------------------------------------------------------
void EU_CreateFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_CreateFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_CreateFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_CreateFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_CreateFrame, target);
}
//----------------------------------------------------------------------------
int EU_CreateFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------