// PX2EU_TimeLineFrame.cpp

#include "PX2EU_TimeLineFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_TimeLineFrame);
PX2_IMPLEMENT_STREAM(EU_TimeLineFrame);
PX2_IMPLEMENT_FACTORY(EU_TimeLineFrame);

//----------------------------------------------------------------------------
EU_TimeLineFrame::EU_TimeLineFrame()
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

	mCanvasTimeLineEdit = new0 EU_CanvasTimeLineEdit();
	AttachChild(mCanvasTimeLineEdit);
	mCanvasTimeLineEdit->LocalTransform.SetTranslateY(-1.0f);
	mCanvasTimeLineEdit->SetAnchorHor(0.0f, 1.0f);
	mCanvasTimeLineEdit->SetAnchorVer(0.0f, 1.0f);
	mCanvasTimeLineEdit->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
EU_TimeLineFrame::~EU_TimeLineFrame()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_TimeLineFrame::EU_TimeLineFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_TimeLineFrame, source);
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_TimeLineFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_TimeLineFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_TimeLineFrame, target);
}
//----------------------------------------------------------------------------
int EU_TimeLineFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------