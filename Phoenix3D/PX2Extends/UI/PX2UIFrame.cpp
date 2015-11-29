// PX2UIFrame.cpp

#include "PX2UIFrame.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIMenuItem.hpp"
#include "PX2UICanvas.hpp"
using namespace PX2;
using namespace std;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, UIFrame);
PX2_IMPLEMENT_STREAM(UIFrame);
PX2_IMPLEMENT_FACTORY(UIFrame);

//----------------------------------------------------------------------------
UIFrame::UIFrame() :
mUICallback(0),
mMemObject(0),
mMemUICallback(0)
{
	SetName("UIFrame");
}
//----------------------------------------------------------------------------
UIFrame::~UIFrame()
{
}
//----------------------------------------------------------------------------
int UIFrame::AttachChild (Movable* child)
{
	int ret = SizeNode::AttachChild(child);

	if (mGridAlignCtrl)
		mGridAlignCtrl->MarkRelatvieChange();

	return ret;
}
//----------------------------------------------------------------------------
void UIFrame::OnSizeChanged()
{
	if (mBackgroundPicBox)
	{
		mBackgroundPicBox->SetSize(mSize);
	}

	SizeNode::OnSizeChanged();
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddBackgroundPicBox()
{
	if (mBackgroundPicBox)
	{
		DetachChild(mBackgroundPicBox);
		mBackgroundPicBox = 0;
	}

	mBackgroundPicBox = new0 UIPicBox();
	AttachChild(mBackgroundPicBox);

	mBackgroundPicBox->SetPivot(Float2(0.5f, 0.5f));
	mBackgroundPicBox->SetSize(mSize);
	mBackgroundPicBox->SetPivot(mPvoit);

	MaterialInstance *mi = mBackgroundPicBox->GetMaterialInstance();
	mi->GetMaterial()->GetPixelShader(0, 0)->SetFilter(0,
		Shader::SF_NEAREST_LINEAR);

	return mBackgroundPicBox;
}
//----------------------------------------------------------------------------
void UIFrame::OnPvoitChanged()
{
	if (mBackgroundPicBox)
		mBackgroundPicBox->SetPivot(mPvoit);

	SizeNode::OnPvoitChanged();
}
//----------------------------------------------------------------------------
void UIFrame::OnUIBeforePicked(int info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void UIFrame::OnUIPicked(int info, Movable *child)
{
	if (!IsEnable())
		return;

	Movable *mov = GetFirstParentDerivedFromType(UICanvas::TYPE);
	UICanvas *uiCanvas = DynamicCast<UICanvas>(mov);
	if (uiCanvas)
	{
		uiCanvas->mPickedFrames.insert(this);
	}

	UIFrame *frame = DynamicCast<UIFrame>(GetParent());
	if (frame)
	{
		frame->OnUIPicked(info, child);
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnUINotPicked(int info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
InputPushTransformController *UIFrame::CreateAddIPTCtrl(bool doResetPlay)
{
	DestoryIPTCtrl();

	mIPTCtrl = new0 InputPushTransformController();
	AttachController(mIPTCtrl);
	mIPTCtrl->SetName("IPTCtrl");

	if (doResetPlay) mIPTCtrl->ResetPlay();

	return mIPTCtrl;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryIPTCtrl()
{
	if (mIPTCtrl)
	{
		DetachController(mIPTCtrl);
	}
}
//----------------------------------------------------------------------------
UIFrameGridAlignControl *UIFrame::CreateAddGridAlignCtrl(bool doResetPlay)
{
	DestoryGridAlignCtrl();

	mGridAlignCtrl = new0 UIFrameGridAlignControl();
	AttachController(mGridAlignCtrl);
	mGridAlignCtrl->SetName("GridAlignControl");

	if (doResetPlay) mGridAlignCtrl->ResetPlay();

	return mGridAlignCtrl;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryGridAlignCtrl()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------------
void UIFrame::RegistProperties()
{
	SizeNode::RegistProperties();

	AddPropertyClass("UIFrame");

	AddProperty("Size", PT_SIZE, mSize);
	AddProperty("BorderSize", PT_SIZE, mBorderSize);

	AddProperty("ScriptHandler", PT_STRING, GetScriptHandler());

	std::vector<std::string> relativesStr;
	relativesStr.push_back("PT_NONE");
	relativesStr.push_back("LT_ZERO");
	relativesStr.push_back("LT_HALF");
	relativesStr.push_back("LT_ONE");
}
//----------------------------------------------------------------------------
void UIFrame::OnPropertyChanged(const PropertyObject &obj)
{
	SizeNode::OnPropertyChanged(obj);

	if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("BorderSize" == obj.Name)
	{
		SetBorderSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("ScriptHandler" == obj.Name)
	{
		SetScriptHandler(PX2_ANY_AS(obj.Data, std::string));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFrame::UIFrame(LoadConstructor value) :
SizeNode(value),
mUICallback(0),
mMemObject(0),
mMemUICallback(0)
{
}
//----------------------------------------------------------------------------
void UIFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	SizeNode::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mUIScriptHandler);

	source.ReadAggregate(mSize);
	source.ReadAggregate(mBorderSize);

	source.ReadBool(mIsAnchorLayoutEnable);
	source.ReadAggregate(mAnchorHor);
	source.ReadAggregate(mAnchorVer);
	source.ReadAggregate(mAnchorParamHor);
	source.ReadAggregate(mAnchorParamVer);

	source.ReadPointer(mIPTCtrl);
	source.ReadPointer(mGridAlignCtrl);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UIFrame::Link(InStream& source)
{
	SizeNode::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIFrame::PostLink()
{
	SizeNode::PostLink();

	RegistToScriptSystemAll();
}
//----------------------------------------------------------------------------
bool UIFrame::Register(OutStream& target) const
{
	if (SizeNode::Register(target))
	{
		if (mIPTCtrl)
			target.Register(mIPTCtrl);

		if (mGridAlignCtrl)
			target.Register(mGridAlignCtrl);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	SizeNode::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mUIScriptHandler);

	target.WriteAggregate(mSize);
	target.WriteAggregate(mBorderSize);

	target.WriteBool(mIsAnchorLayoutEnable);
	target.WriteAggregate(mAnchorHor);
	target.WriteAggregate(mAnchorVer);
	target.WriteAggregate(mAnchorParamHor);
	target.WriteAggregate(mAnchorParamVer);

	target.WritePointer(mIPTCtrl);
	target.WritePointer(mGridAlignCtrl);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UIFrame::GetStreamingSize(Stream &stream) const
{
	int size = SizeNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_STRINGSIZE(mUIScriptHandler);

	size += sizeof(mSize);
	size += sizeof(mBorderSize);

	size += PX2_BOOLSIZE(mIsAnchorLayoutEnable);
	size += sizeof(mAnchorHor);
	size += sizeof(mAnchorVer);
	size += sizeof(mAnchorParamHor);
	size += sizeof(mAnchorParamVer);
	
	size += PX2_POINTERSIZE(mIPTCtrl);
	size += PX2_POINTERSIZE(mGridAlignCtrl);

	return size;
}
//----------------------------------------------------------------------------