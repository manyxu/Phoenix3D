// EU_InspectorFrame.cpp

#include "PX2EU_InspectorFrame.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_InspectorFrame);
PX2_IMPLEMENT_STREAM(EU_InspectorFrame);
PX2_IMPLEMENT_FACTORY(EU_InspectorFrame);

//----------------------------------------------------------------------------
EU_InspectorFrame::EU_InspectorFrame()
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

	Sizef butSize = Sizef(PX2_UISM.Size_ToolBarBut, PX2_UISM.Size_ToolBarBut);
	float butSpace = 2.0f;
	Sizef spliterSize = Sizef(4.0f,
		PX2_UISM.Size_ToolBar - 2);

	float addButPos = 0.0f;

	// edit type
	addButPos = butSize.Width / 2.0f;
	UIButton *butProperty = new0 UIButton();
	mToolFrame->AttachChild(butProperty);
	butProperty->LocalTransform.SetTranslateY(-1.0f);
	butProperty->SetAnchorHor(0.0f, 0.0f);
	butProperty->SetAnchorVer(0.5f, 0.5f);
	butProperty->SetAnchorParamHor(addButPos, 0.0f);
	butProperty->SetSize(butSize);
	butProperty->SetName("Property");
	butProperty->CreateAddText("Prop");
	butProperty->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butProperty->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	addButPos += butSpace + butSize.Width;
	UIButton *butEdit = new0 UIButton();
	mToolFrame->AttachChild(butEdit);
	butEdit->LocalTransform.SetTranslateY(-1.0f);
	butEdit->SetAnchorHor(0.0f, 0.0f);
	butEdit->SetAnchorVer(0.5f, 0.5f);
	butEdit->SetAnchorParamHor(addButPos, 0.0f);
	butEdit->SetSize(butSize);
	butEdit->SetName("Edit");
	butEdit->CreateAddText("Edit");
	butEdit->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butEdit->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

	mPropertyGrid = new0 EU_PropertyGrid();
	AttachChild(mPropertyGrid);
	mPropertyGrid->SetAnchorHor(0.0f, 1.0f);
	mPropertyGrid->SetAnchorVer(0.0f, 1.0f);
	mPropertyGrid->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);
}
//----------------------------------------------------------------------------
EU_InspectorFrame::~EU_InspectorFrame()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_InspectorFrame::EU_InspectorFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_InspectorFrame, source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_InspectorFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_InspectorFrame, target);
}
//----------------------------------------------------------------------------
int EU_InspectorFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------