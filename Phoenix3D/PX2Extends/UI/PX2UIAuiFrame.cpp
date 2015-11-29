// PX2EU_Frame.cpp

#include "PX2UIAuiFrame.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIAuiFrame);
PX2_IMPLEMENT_STREAM(UIAuiFrame);
PX2_IMPLEMENT_FACTORY(UIAuiFrame);

//----------------------------------------------------------------------------
UIAuiFrame::UIAuiFrame()
{
	float spliterSize = PX2_UISM.Size_Splitter;

	// left
	SpLeft = new0 UISplitterFrame(false);
	AttachChild(SpLeft);
	SpLeft->SetName("SpLeft");
	SpLeft->LocalTransform.SetTranslateY(0.0f);
	SpLeft->SetAnchorHor(Float2(0.0f, 0.0f));
	SpLeft->SetAnchorParamHor(Float2(0.0f, 0.0f));
	SpLeft->SetAnchorVer(Float2(0.0f, 1.0f));
	SpLeft->SetAnchorParamVer(Float2(spliterSize, spliterSize));
	SpLeft->SetPivot(0.0f, 0.5f);
	SpLeft->SetDragable(false);

	// right
	SpRight = new0 UISplitterFrame(false);
	AttachChild(SpRight);
	SpRight->SetName("SpRight");
	SpRight->LocalTransform.SetTranslateY(0.0f);
	SpRight->SetAnchorHor(Float2(1.0f, 1.0f));
	SpRight->SetAnchorParamHor(Float2(0.0f, 0.0f));
	SpRight->SetAnchorVer(Float2(0.0f, 1.0f));
	SpRight->SetAnchorParamVer(Float2(spliterSize, spliterSize));
	SpRight->SetPivot(1.0f, 0.5f);
	SpRight->SetDragable(false);

	// bottom
	SpBottom = new0 UISplitterFrame(true);
	AttachChild(SpBottom);
	SpBottom->SetName("SpBottom");
	SpBottom->LocalTransform.SetTranslateY(0.0f);
	SpBottom->SetAnchorHor(Float2(0.0f, 1.0f));
	SpBottom->SetAnchorParamHor(Float2(0.0f, 0.0f));
	SpBottom->SetAnchorVer(Float2(0.0f, 0.0f));
	SpBottom->SetAnchorParamVer(Float2(0.0f, 0.0f));
	SpBottom->SetPivot(0.5f, 0.0f);
	SpBottom->SetDragable(false);

	// top
	SpTop = new0 UISplitterFrame(true);
	AttachChild(SpTop);
	SpTop->SetName("SpTop");
	SpTop->LocalTransform.SetTranslateY(0.0f);
	SpTop->SetAnchorHor(Float2(0.0f, 1.0f));
	SpTop->SetAnchorParamHor(Float2(0.0f, 0.0f));
	SpTop->SetAnchorVer(Float2(1.0f, 1.0f));
	SpTop->SetAnchorParamVer(Float2(0.0f, 0.0f));
	SpTop->SetPivot(0.5f, 1.0f);
	SpTop->SetDragable(false);

	SpLeft->SetLinkFrame0(SpBottom);
	SpLeft->SetLinkFrame1(SpTop);
	SpRight->SetLinkFrame0(SpBottom);
	SpRight->SetLinkFrame1(SpTop);
	SpBottom->SetLinkFrame0(SpLeft);
	SpBottom->SetLinkFrame1(SpRight);
	SpTop->SetLinkFrame0(SpLeft);
	SpTop->SetLinkFrame1(SpRight);

	// default
	AuiBlockDefault = new0 UIAuiBlockFrame(UILPT_CENTER);
	AttachChild(AuiBlockDefault);
	AuiBlockDefault->SetSideFrameHor0(SpBottom);
	AuiBlockDefault->SetSideFrameHor1(SpTop);
	AuiBlockDefault->SetSideFrameVer0(SpLeft);
	AuiBlockDefault->SetSideFrameVer1(SpRight);
}
//----------------------------------------------------------------------------
UIAuiFrame::~UIAuiFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIAuiFrame::UIAuiFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIAuiFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIAuiFrame, source);
}
//----------------------------------------------------------------------------
void UIAuiFrame::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIAuiFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIAuiFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIAuiFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIAuiFrame, target);
}
//----------------------------------------------------------------------------
int UIAuiFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------