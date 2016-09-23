// PX2EU_BPFrame.cpp

#include "PX2EU_BPFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2Math.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_BPFrame);
PX2_IMPLEMENT_STREAM(EU_BPFrame);
PX2_IMPLEMENT_FACTORY(EU_BPFrame);

//----------------------------------------------------------------------------
EU_BPFrame::EU_BPFrame()
{
}
//----------------------------------------------------------------------------
EU_BPFrame::~EU_BPFrame()
{
}
//----------------------------------------------------------------------------
UICanvas *EU_BPFrame::CreateChildUICanvas()
{
	std::string strUICanvas = "BP" + GetName();

	mBPUICanvas = new0 EU_CanvasBP();
	AttachChild(mBPUICanvas);
	mBPUICanvas->SetClearFlag(true, true, true);
	mBPUICanvas->SetClearColor(Float4::MakeColor(195, 195, 195, 255));
	mBPUICanvas->SetAnchorHor(0.0f, 1.0f);
	mBPUICanvas->SetAnchorVer(0.0f, 1.0f);

	return mBPUICanvas;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_BPFrame::EU_BPFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_BPFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_BPFrame, source);
}
//----------------------------------------------------------------------------
void EU_BPFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_BPFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_BPFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_BPFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_BPFrame, target);
}
//----------------------------------------------------------------------------
int EU_BPFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------