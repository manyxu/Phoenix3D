// PX2EU_BluePrintFrame.cpp

#include "PX2EU_BluePrintFrame.hpp"
#include "PX2EU_BPFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UICanvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_BluePrintFrame);
PX2_IMPLEMENT_STREAM(EU_BluePrintFrame);
PX2_IMPLEMENT_FACTORY(EU_BluePrintFrame);

//----------------------------------------------------------------------------
EU_BluePrintFrame::EU_BluePrintFrame()
{
	mTableFrame = new0 UITabFrame();
	AttachChild(mTableFrame);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	mTableFrame->SetLayoutPos(UITabFrame::LPT_TOP);
	mTableFrame->SetTabWidth(80.0f);
	mTableFrame->SetSkinAui(false);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);
	AddBPFrame("BP0");
	AddBPFrame("BP1");
	AddBPFrame("BP2");
	mTableFrame->SetActiveTab("BP0");
}
//----------------------------------------------------------------------------
EU_BluePrintFrame::~EU_BluePrintFrame()
{
}
//----------------------------------------------------------------------------
EU_BPFrame *EU_BluePrintFrame::AddBPFrame(const std::string &name)
{
	EU_BPFrame *bpFrame = new0 EU_BPFrame();
	mTableFrame->AddTab(name, bpFrame);
	bpFrame->LocalTransform.SetTranslateY(-1.0f);
	bpFrame->SetAnchorHor(0.0f, 1.0f);
	bpFrame->SetAnchorVer(0.0f, 1.0f);
	bpFrame->SetName(name);
	bpFrame->CreateChildUICanvas();

	return bpFrame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_BluePrintFrame::EU_BluePrintFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_BluePrintFrame, source);
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_BluePrintFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_BluePrintFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_BluePrintFrame, target);
}
//----------------------------------------------------------------------------
int EU_BluePrintFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------