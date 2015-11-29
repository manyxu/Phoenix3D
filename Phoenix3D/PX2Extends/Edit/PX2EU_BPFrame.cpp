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
	mToolFrame->SetSize(0.0f, PX2_UISM.Size_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(PX2_UISM.Color_ToolBar);
}
//----------------------------------------------------------------------------
EU_BPFrame::~EU_BPFrame()
{
}
//----------------------------------------------------------------------------
void EU_BPFrame::CreateChildUICanvas()
{
	std::string strUICanvas = "BP" + GetName();

	mBPUICanvas = new0 UICanvas(20);
	mBPUICanvas->SetPriority(-1.0);
	PX2_GR.AddCanvas(strUICanvas.c_str(), mBPUICanvas);
	AttachChild(mBPUICanvas);
	mBPUICanvas->SetRenderer(Renderer::GetDefaultRenderer());
	mBPUICanvas->SetBeforeDrawClear(true, true, true);
	mBPUICanvas->SetClearColor(Float4(Mathf::UnitRandom(), 
		Mathf::UnitRandom(), Mathf::UnitRandom(), 1.0f));
	mBPUICanvas->SetAnchorHor(0.0f, 1.0f);
	mBPUICanvas->SetAnchorVer(0.0f, 1.0f);
	mBPUICanvas->SetAnchorParamVer(0.0f, PX2_UISM.Size_ToolBar);
	mBPUICanvas->CreateAndAddCanvasController();

	UIFrame *frame = new0 UIFrame();
	mBPUICanvas->AttachChild(frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(0.0f, 1.0f);

	UIPicBox *backPicBox = new0 UIPicBox();
	frame->AttachChild(backPicBox);
	backPicBox->SetSize(2048.0f, 2048.0f);
	backPicBox->SetTexture("DataNIRVANA2/images/common/backgroundgrid.png");
	backPicBox->SetColor(Float3::MakeColor(200.0f, 200.0f, 200.0f));
	backPicBox->SetUVRepeat(Float2(40.0f, 40.0f));
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