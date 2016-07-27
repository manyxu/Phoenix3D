// PX2EU_SimuFrame.cpp

#include "PX2EU_SimuFrame.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_SimuFrame);
PX2_IMPLEMENT_STREAM(EU_SimuFrame);
PX2_IMPLEMENT_FACTORY(EU_SimuFrame);

//----------------------------------------------------------------------------
EU_SimuFrame::EU_SimuFrame()
{
	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	mToolFrame->SetSize(Sizef(0.0f, PX2_UISM.Size_ToolBar));

	mCanvasStageSimu = new0 EU_CanvasStageSimu();
	mCanvasStageSimu->LocalTransform.SetTranslateY(-1.0f);
	AttachChild(mCanvasStageSimu);
	mCanvasStageSimu->SetAnchorHor(0.0f, 1.0f);
	mCanvasStageSimu->SetAnchorVer(0.0f, 1.0f);
	mCanvasStageSimu->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
EU_SimuFrame::~EU_SimuFrame()
{
}
//----------------------------------------------------------------------------
void EU_SimuFrame::Visit(Object *obj, int info)
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_SimuFrame::EU_SimuFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_SimuFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_SimuFrame, source);
}
//----------------------------------------------------------------------------
void EU_SimuFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_SimuFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_SimuFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_SimuFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_SimuFrame, target);
}
//----------------------------------------------------------------------------
int EU_SimuFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------