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
	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-2.0f);
	mToolFrame->SetSize(0.0f, PX2_UISM.Size_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
}
//----------------------------------------------------------------------------
EU_BPFrame::~EU_BPFrame()
{
}
//----------------------------------------------------------------------------
UICanvas *EU_BPFrame::CreateChildUICanvas()
{
	//std::string strUICanvas = "BP" + GetName();

	//mBPUICanvas = new0 EU_CanvasBP();
	//AttachChild(mBPUICanvas);
	//mBPUICanvas->SetBeforeDrawClear(true, true, true);
	//mBPUICanvas->SetClearColor(Float4::MakeColor(239, 228, 176, 255));
	//mBPUICanvas->SetAnchorHor(0.0f, 1.0f);
	//mBPUICanvas->SetAnchorVer(0.0f, 1.0f);
	//mBPUICanvas->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
	//mBPUICanvas->CreateAndAddCanvasController();

	//UIFrame *frame = new0 UIFrame();
	//mBPUICanvas->AttachChild(frame);
	//frame->SetAnchorHor(0.0f, 1.0f);
	//frame->SetAnchorVer(0.0f, 1.0f);

	//return mBPUICanvas;

	return 0;
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