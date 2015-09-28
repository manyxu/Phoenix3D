// PX2UIFrame.cpp

#include "PX2UIFrame.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIView.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIMenuItem.hpp"
using namespace PX2;
using namespace std;

PX2_IMPLEMENT_RTTI(PX2, Node, UIFrame);
PX2_IMPLEMENT_STREAM(UIFrame);
PX2_IMPLEMENT_FACTORY(UIFrame);

//----------------------------------------------------------------------------
UIFrame::UIFrame() :
mUICallback(0),
mIsLayoutChanged(true),
mUIView(0),
mIsAnchorLayoutEnable(true)
{
	SetName("UIFrame");

	mPvoit = Float2(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
UIFrame::~UIFrame()
{
}
//----------------------------------------------------------------------------
int UIFrame::AttachChild (Movable* child)
{
	int ret = Node::AttachChild(child);

	if (mGridAlignCtrl)
		mGridAlignCtrl->MarkRelatvieChange();

	return ret;
}
//----------------------------------------------------------------------------
void UIFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsLayoutChanged && mIsAnchorLayoutEnable)
		UpdateLayout();

	Node::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIFrame::SetSize(float width, float height)
{
	SetSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void UIFrame::SetSize(const Sizef &size)
{
	mSize = size;
	OnSizeChanged();
}
//----------------------------------------------------------------------------
void sExecuteFun(Movable *mov, Any *data)
{
	UIFrame *frame = DynamicCast<UIFrame>(mov);
	if (!frame) return;

	for (int i = 0; i < frame->GetNumChildren(); i++)
	{
		UIFrame *childUIFrame = DynamicCast<UIFrame>(frame->GetChild(i));
		if (childUIFrame)
		{
			childUIFrame->MarkRelatvieChange();
		}
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnSizeChanged()
{
	if (mBackgroundPicBox)
	{
		mBackgroundPicBox->SetSize(mSize);
	}

	Node::TravelExecute(this, sExecuteFun);
}
//----------------------------------------------------------------------------
void UIFrame::SetWidth(float width)
{
	SetSize(Sizef(width, mSize.Height));
}
//----------------------------------------------------------------------------
void UIFrame::SetHeight(float height)
{
	SetSize(Sizef(mSize.Width, height));
}
//----------------------------------------------------------------------------
void UIFrame::SetBorderSize(float width, float height)
{
	SetBorderSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void UIFrame::SetBorderSize(const Sizef &size)
{
	mBorderSize = size;

	OnBorderSizeChanged();
}
//----------------------------------------------------------------------------
void UIFrame::SetBorderWidth(float width)
{
	SetBorderSize(Sizef(width, mSize.Height));
}
//----------------------------------------------------------------------------
void UIFrame::SetBorderHeight(float height)
{
	SetBorderSize(Sizef(mSize.Width, height));
}
//----------------------------------------------------------------------------
void UIFrame::OnBorderSizeChanged()
{
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddBackgroundPicBox()
{
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
void UIFrame::SetPvoit(float x, float y)
{
	SetPvoit(Float2(x, y));
}
//----------------------------------------------------------------------------
void UIFrame::SetPvoit(const Float2 &pvoit)
{
	mPvoit = pvoit;

	if (mBackgroundPicBox)
		mBackgroundPicBox->SetPivot(mPvoit);

	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::OnChildPicked(int info, Movable *child)
{
	if (!IsEnable())
		return;

	UIFrame *frame = DynamicCast<UIFrame>(GetParent());
	if (frame)
	{
		frame->OnChildPicked(info, child);
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnChildUIAfterPicked(int info, Movable *child)
{
	if (!IsEnable())
		return;

	UIFrame *frame = DynamicCast<UIFrame>(GetParent());
	if (frame)
	{
		frame->OnChildUIAfterPicked(info, child);
	}
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
void UIFrame::EnableAnchorLayout(bool enable)
{
	mIsAnchorLayoutEnable = enable;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorHor(float anchorX, float anchorY)
{
	SetAnchorHor(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorHor(const Float2 &anchor)
{
	mAnchorHor = anchor;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorVer(float anchorX, float anchorY)
{
	SetAnchorVer(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorVer(const Float2 &anchor)
{
	mAnchorVer = anchor;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorParamHor(float param0, float param1)
{
	SetAnchorParamHor(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorParamHor(const Float2 &param)
{
	mAnchorParamHor = param;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorParamVer(float param0, float param1)
{
	SetAnchorParamVer(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void UIFrame::SetAnchorParamVer(const Float2 &param)
{
	mAnchorParamVer = param;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::MarkRelatvieChange()
{
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrame::UpdateLayout()
{
	UIView *uiView = GetUIView();
	UIFrame *parent = DynamicCast<UIFrame>(this->GetParent());

	// parent pvoit决定了从那一点作为原点，计算当前Frame原点位置
	// 例如
	// (0.0f, 0.0f)是左下角
	// (0.5f, 0.5f)是中心点	
	Sizef parentSize;
	AVector parentLeftBottomOffset;
	if (uiView)
	{
		parentSize = uiView->GetSize();
		parentLeftBottomOffset = AVector::ZERO;
	}
	else if (parent)
	{
		parentSize = parent->GetSize();
		parentLeftBottomOffset = parent->LeftBottomCornerOffset;
	}

	APoint localPos = LocalTransform.GetTranslate();
	Sizef localSize = GetSize();
	const Float2 &pvoit = GetPvoit();

	APoint newPos;
	Sizef newSize = localSize;
	if (uiView || parent)
	{
		float width = 0.0f;
		float height = 0.0f;

		bool isHEqual = (mAnchorHor[0] == mAnchorHor[1]);
		bool isVEqual = (mAnchorVer[0] == mAnchorVer[1]);
		
		if (isHEqual)
		{
			width = localSize.Width;
			newPos.X() = parentLeftBottomOffset.X() + 
				parentSize.Width * mAnchorHor[0] + mAnchorParamHor[0] ;

			LeftBottomCornerOffset.X() = - width * pvoit[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			width = parentSize.Width * (mAnchorHor[1] - mAnchorHor[0]) - mAnchorParamHor[0] - mAnchorParamHor[1];
			newPos.X() = parentLeftBottomOffset.X() + mAnchorParamHor[0] + width / 2.0f;

			LeftBottomCornerOffset.X() = -width / 2.0f;
		}
		
		if (isVEqual)
		{
			height = localSize.Height;
			newPos.Z() = parentLeftBottomOffset.Z() + 
				parentSize.Height * mAnchorVer[0] + mAnchorParamVer[0];

			LeftBottomCornerOffset.Z() = - height * pvoit[1];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			height = parentSize.Height * (mAnchorVer[1] - mAnchorVer[0]) - mAnchorParamVer[0] - mAnchorParamVer[1];
			newPos.Z() = parentLeftBottomOffset.Z() + mAnchorParamVer[0] + height / 2.0f;

			LeftBottomCornerOffset.Z() = -height / 2.0f;
		}

		newSize = Sizef(width, height);
	}

	newPos.Y() = localPos.Y();
	LocalTransform.SetTranslate(newPos);

	if (newSize != localSize)
	{
		SetSize(newSize);
	}

	mIsLayoutChanged = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------------
void UIFrame::RegistProperties()
{
	Node::RegistProperties();

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
	Node::OnPropertyChanged(obj);

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
Node(value),
mUICallback(0),
mIsLayoutChanged(true),
mUIView(0),
mIsAnchorLayoutEnable(true)
{
}
//----------------------------------------------------------------------------
void UIFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
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
	Node::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIFrame::PostLink()
{
	Node::PostLink();

	RegistToScriptSystemAll();
}
//----------------------------------------------------------------------------
bool UIFrame::Register(OutStream& target) const
{
	if (Node::Register(target))
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

	Node::Save(target);
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
	int size = Node::GetStreamingSize(stream);
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