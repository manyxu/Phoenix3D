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
	UIButton *butRes = new0 UIButton();
	mToolFrame->AttachChild(butRes);
	butRes->LocalTransform.SetTranslateY(-1.0f);
	butRes->SetAnchorHor(0.0f, 0.0f);
	butRes->SetAnchorVer(0.5f, 0.5f);
	butRes->SetAnchorParamHor(addButPos, 0.0f);
	butRes->SetSize(butSize);
	butRes->SetName("Res");
	butRes->CreateAddText("Res");
	butRes->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butRes->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
	butRes->SetMemUICallback(this, (UIFrame::MemUICallback)(&EU_InspectorFrame::_PropUICallback));

	addButPos += butSpace + butSize.Width;
	UIButton *butProp = new0 UIButton();
	mToolFrame->AttachChild(butProp);
	butProp->LocalTransform.SetTranslateY(-1.0f);
	butProp->SetAnchorHor(0.0f, 0.0f);
	butProp->SetAnchorVer(0.5f, 0.5f);
	butProp->SetAnchorParamHor(addButPos, 0.0f);
	butProp->SetSize(butSize);
	butProp->SetName("Prop");
	butProp->CreateAddText("Prop");
	butProp->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	butProp->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
	butProp->SetMemUICallback(this, (UIFrame::MemUICallback)(&EU_InspectorFrame::_PropUICallback));

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
	butEdit->SetMemUICallback(this, (UIFrame::MemUICallback)(&EU_InspectorFrame::_PropUICallback));

	mPropertyGrid = new0 EU_PropertyGrid();
	AttachChild(mPropertyGrid);
	mPropertyGrid->SetAnchorHor(0.0f, 1.0f);
	mPropertyGrid->SetAnchorVer(0.0f, 1.0f);
	mPropertyGrid->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

	mInspectorFrameRes = new0 EU_InspectorFrameRes();
	AttachChild(mInspectorFrameRes);
	mInspectorFrameRes->SetAnchorHor(0.0f, 1.0f);
	mInspectorFrameRes->SetAnchorVer(0.0f, 1.0f);
	mInspectorFrameRes->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

	mEditFrame = new0 EU_EditFrame();
	AttachChild(mEditFrame);
	mEditFrame->SetAnchorHor(0.0f, 1.0f);
	mEditFrame->SetAnchorVer(0.0f, 1.0f);
	mEditFrame->SetAnchorParamVer(0.0f, -PX2_UISM.Size_ToolBar);

	SetInspType(IT_RES);
}
//----------------------------------------------------------------------------
EU_InspectorFrame::~EU_InspectorFrame()
{
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::SetInspType(InspType type)
{
	mInspType = type;

	if (IT_PROP == mInspType)
	{
		mPropertyGrid->Show(true);
		mInspectorFrameRes->Show(false);
		mEditFrame->Show(false);
	}
	else if (IT_RES == mInspType)
	{
		mPropertyGrid->Show(false);
		mInspectorFrameRes->Show(true);
		mEditFrame->Show(false);
	}
	else if (IT_EDIT == mInspType)
	{
		mPropertyGrid->Show(false);
		mInspectorFrameRes->Show(false);
		mEditFrame->Show(true);
	}
}
//----------------------------------------------------------------------------
EU_InspectorFrame::InspType EU_InspectorFrame::GetInspType() const
{
	return mInspType;
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::_PropUICallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but)
	{
		if (UICT_RELEASED == type)
		{
			if ("Res" == name)
			{
				SetInspType(IT_RES);
			}
			else if ("Prop" == name)
			{
				SetInspType(IT_PROP);
			}
			else if ("Edit" == name)
			{
				SetInspType(IT_EDIT);
			}
		}
	}
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