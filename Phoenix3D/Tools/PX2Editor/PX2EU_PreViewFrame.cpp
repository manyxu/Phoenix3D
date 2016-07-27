// PX2EU_PreViewFrame.cpp

#include "PX2EU_PreViewFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIComboBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_PreViewFrame);
PX2_IMPLEMENT_STREAM(EU_PreViewFrame);
PX2_IMPLEMENT_FACTORY(EU_PreViewFrame);

//----------------------------------------------------------------------------
EU_PreViewFrame::EU_PreViewFrame()
{
	float toolBarSize = PX2_UISM.Size_ToolBar;

	mToolFrame = new0 UIFrame();
	AttachChild(mToolFrame);
	mToolFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mToolFrame->CreateAddBackgroundPicBox();
	picBox->SetColor(PX2_UISM.Color_ToolBar);
	mToolFrame->SetAnchorHor(0.0f, 1.0f);
	mToolFrame->SetAnchorVer(1.0f, 1.0f);
	mToolFrame->SetPivot(0.5f, 1.0f);
	mToolFrame->SetSize(Sizef(0.0f, toolBarSize));

	float comboWidth = 50.0f;
	UIComboBox *comboBox = new0 UIComboBox();
	mToolFrame->AttachChild(comboBox);	
	comboBox->LocalTransform.SetTranslateY(-1.0f);
	comboBox->SetSize(comboWidth, toolBarSize);
	comboBox->SetAnchorHor(0.0f, 0.0f);
	comboBox->SetAnchorParamHor(comboWidth/2.0f, 0.0f);
	comboBox->SetAnchorVer(0.5f, 0.5f);
	comboBox->AddChooseStr("1");
	comboBox->AddChooseStr("2");
	comboBox->AddChooseStr("3");
	comboBox->SetChooseListHeightSameWithChooses();
	comboBox->Choose(0);

	mCanvasPreView = new0 EU_CanvasPreView();
	mCanvasPreView->SetName("CanvasPreView");
	mCanvasPreView->LocalTransform.SetTranslateY(-1.0f);
	AttachChild(mCanvasPreView);
	mCanvasPreView->LocalTransform.SetTranslateY(-1.0f);
	mCanvasPreView->SetAnchorHor(0.0f, 1.0f);
	mCanvasPreView->SetAnchorVer(0.0f, 1.0f);
	mCanvasPreView->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
EU_PreViewFrame::~EU_PreViewFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_PreViewFrame::EU_PreViewFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_PreViewFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_PreViewFrame, source);
}
//----------------------------------------------------------------------------
void EU_PreViewFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_PreViewFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_PreViewFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_PreViewFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_PreViewFrame, target);
}
//----------------------------------------------------------------------------
int EU_PreViewFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------