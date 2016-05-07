// PX2UIFrame.cpp

#include "PX2UIFrame.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UICanvas.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIMenuItem.hpp"
#include "PX2UICanvas.hpp"
#include "PX2FunObject.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;
using namespace std;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, UIFrame);
PX2_IMPLEMENT_STREAM(UIFrame);
PX2_IMPLEMENT_FACTORY(UIFrame);

//----------------------------------------------------------------------------
UIFrame::UIFrame() :
mBelongUICanvas(0),
mUICallback(0),
mIsWidget(false),
mMemObject(0),
mMemUICallback(0),
mActivatedColor(Float3::WHITE),
mActivatedAlpha(1.0f),
mActivatedBrightness(1.0f),
mIsUIChildPickOnlyInSizeRange(true)
{
	SetActivate(false);
	SetName("UIFrame");
}
//----------------------------------------------------------------------------
UIFrame::~UIFrame()
{
}
//----------------------------------------------------------------------------
void UIFrame::TravelExecuteSetCanvas(Movable *mov, Any *data, bool &goOn)
{
	PX2_UNUSED(goOn);

	UICanvas *uiCanvas = DynamicCast<UICanvas>(mov);
	UIFrame *uiFrame = DynamicCast<UIFrame>(mov);
	if (uiCanvas)
	{
		UICanvas *canvas = PX2_ANY_AS(*data, UICanvas*);
		if (canvas != uiCanvas)
		{
			goOn = false;
		}
	}
	if (uiFrame)
	{
		UICanvas *canvas = PX2_ANY_AS(*data, UICanvas*);
		uiFrame->_SetBelongUICanvas(canvas);
	}
}
//----------------------------------------------------------------------------
void UIFrame::SetBelongUICanvas(UICanvas *canvas)
{
	Any canvasData = canvas;
	Node::TravelExecute(this, TravelExecuteSetCanvas, &canvasData);
}
//----------------------------------------------------------------------------
void UIFrame::_SetBelongUICanvas(UICanvas *canvas)
{
	if (mBelongUICanvas)
	{
		if (mIsWidget)
			mBelongUICanvas->_RemovePickWidget(this);
	}

	mBelongUICanvas = canvas;

	if (mBelongUICanvas)
	{
		if (mIsWidget)
			mBelongUICanvas->_AddPickWidget(this);
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnChildAttached(Movable *child)
{
	SizeNode::OnChildAttached(child);

	if (mBelongUICanvas)
	{
		Any canvasData = mBelongUICanvas;
		Node::TravelExecute(this, TravelExecuteSetCanvas, &canvasData);
	}

	UICanvas *canvas = GetFirstParentDerivedFromType<UICanvas>();
	if (canvas)
	{
		canvas->SetNeedAdjustChildrenMask(true);
	}

	if (mGridAlignCtrl)
		mGridAlignCtrl->MarkLayoutChanged();
}
//----------------------------------------------------------------------------
void UIFrame::OnChildDetach(Movable *child)
{
	SizeNode::OnChildDetach(child);

	Any canvasData = (UICanvas*)0;
	Node::TravelExecute(child, TravelExecuteSetCanvas, &canvasData);

	UICanvas *canvas = GetFirstParentDerivedFromType<UICanvas>();
	if (canvas)
	{
		canvas->SetNeedAdjustChildrenMask(true);
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnSizeChanged()
{
	if (mBackgroundPicBox)
	{
		mBackgroundPicBox->SetSize(mSize);
	}

	if (mMaskPicBox)
	{
		mMaskPicBox->SetSize(mSize);
	}

	SizeNode::OnSizeChanged();
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddBackgroundPicBox(bool setWhite)
{
	if (mBackgroundPicBox)
	{
		DetachChild(mBackgroundPicBox);
		mBackgroundPicBox = 0;
	}

	mBackgroundPicBox = new0 UIPicBox();
	AttachChild(mBackgroundPicBox);
	mBackgroundPicBox->SetName("BackPicBox");

	mBackgroundPicBox->SetSize(mSize);
	mBackgroundPicBox->SetPivot(mPvoit);

	MaterialInstance *mi = mBackgroundPicBox->GetMaterialInstance();
	mi->GetMaterial()->GetPixelShader(0, 0)->SetFilter(0,
		Shader::SF_NEAREST_LINEAR);

	if (setWhite)
	{
		mBackgroundPicBox->SetTexture("Data/engine/white.png");
	}

	return mBackgroundPicBox;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryBackgroundPicBox()
{
	if (mBackgroundPicBox)
	{
		DetachChild(mBackgroundPicBox);
		mBackgroundPicBox = 0;
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnPvoitChanged()
{
	if (mBackgroundPicBox)
		mBackgroundPicBox->SetPivot(mPvoit);

	if (mMaskPicBox)
		mMaskPicBox->SetPivot(mPvoit);

	SizeNode::OnPvoitChanged();
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateColor(const Float3 &color)
{
	mActivatedColor = color;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateAlpha(float alpha)
{
	mActivatedAlpha = alpha;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateBrightness(float brightness)
{
	mActivatedBrightness = brightness;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetUIChildPickOnlyInSizeRange(bool onlyInRange)
{
	mIsUIChildPickOnlyInSizeRange = onlyInRange;
}
//----------------------------------------------------------------------------
bool UIFrame::IsUIChildPickOnlyInSizeRange() const
{
	return mIsUIChildPickOnlyInSizeRange;
}
//----------------------------------------------------------------------------
void UIFrame::PreUIPick(const UIInputData &inputData, UICanvas *canvas)
{
	if (!IsShow())
		return;

	Rectf rect = GetWorldRect();
	bool isPosInSizeRange = rect.IsInsize(inputData.WorldPos.X(),
		inputData.WorldPos.Z());

	if (mIsWidget)
	{
		if (isPosInSizeRange)
			canvas->_AddInRangePickWidget(this);
	}

	bool pickChild = true;
	if (mIsUIChildPickOnlyInSizeRange && !isPosInSizeRange)
		pickChild = false;

	if (pickChild)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			UIFrame *childFrame = DynamicCast<UIFrame>(GetChild(i));
			if (childFrame)
			{
				childFrame->PreUIPick(inputData, canvas);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnUINotPicked(const UIInputData &inputData)
{
	PX2_UNUSED(inputData);
}
//----------------------------------------------------------------------------
void UIFrame::OnUIPicked(const UIInputData &inputData)
{
	PX2_UNUSED(inputData);
}
//----------------------------------------------------------------------------
void UIFrame::SetWidget(bool isWidget)
{
	mIsWidget = isWidget;
}
//----------------------------------------------------------------------------
void UIFrame::CallString(const std::string &funStr, const char *format, ...)
{
	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (!sm) return;

	va_list argptr;
	va_start(argptr, format);
	PX2_SC_AS->CallObjectFuntionValist(funStr, this, format, argptr);
	va_end(argptr);
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddMask()
{
	DestoryMask();

	mMaskPicBox = new0 UIPicBox();
	AttachChild(mMaskPicBox);
	mMaskPicBox->SetName("MaskPicBox");
	Material *mtl = mMaskPicBox->GetMaterialInstance()->GetMaterial();
	OffsetProperty *op = mtl->GetOffsetProperty(0, 0);
	op->AllowRed = false;
	op->AllowGreen = false;
	op->AllowBlue = false;
	op->AllowAlpha = false;
	StencilProperty *sp = mtl->GetStencilProperty(0, 0);
	sp->Enabled = true;

	mMaskPicBox->LocalTransform.SetTranslateY(0.2f);
	mMaskPicBox->SetSize(mSize);
	mMaskPicBox->SetPivot(mPvoit);

	UICanvas *canvas = GetFirstParentDerivedFromType<UICanvas>();
	if (canvas)
	{
		canvas->SetNeedAdjustChildrenMask(true);
	}

	return mMaskPicBox;
}
//----------------------------------------------------------------------------
void UIFrame::SetMaskVal(int maskVal)
{
	mMaskVal = maskVal;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryMask()
{
	if (!mMaskPicBox) return;

	DetachChild(mMaskPicBox);
	mMaskPicBox = 0;

	UICanvas *canvas = GetFirstParentDerivedFromType<UICanvas>();
	if (canvas)
	{
		canvas->SetNeedAdjustChildrenMask(true);
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

//----------------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------------
void UIFrame::RegistProperties()
{
	SizeNode::RegistProperties();

	AddPropertyClass("UIFrame");

	AddProperty("Size", PT_SIZE, mSize);

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
	else if ("ScriptHandler" == obj.Name)
	{
		SetScriptHandler(PX2_ANY_AS(obj.Data, std::string));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Object Functions
//----------------------------------------------------------------------------
FunObject *UIFrame::RegistClassFunctions()
{
	FunObject *parFunObject = SizeNode::RegistClassFunctions();

	FunObject *thisFunObj = parFunObject->GetAddClass("UIFrame");

	{
		FunObjectPtr funObj = new0 FunObject;
		funObj->FunName = "SetScriptHandler";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_handlerstr", FPT_STRING, std::string(""));
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject;
		funObj->FunName = "GetScriptHandler";
		funObj->AddInput("handler", FPT_POINTER_THIS, (Object*)0);
		funObj->AddOutput("ot_handlerstr", FPT_STRING, std::string("notvalied"));
		thisFunObj->AddFunObject(funObj);
	}

	return thisFunObj;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFrame::UIFrame(LoadConstructor value) :
SizeNode(value),
mBelongUICanvas(0),
mIsWidget(false),
mUICallback(0),
mMemObject(0),
mMemUICallback(0),
mActivatedColor(Float3::WHITE),
mActivatedAlpha(1.0f),
mActivatedBrightness(1.0f),
mIsUIChildPickOnlyInSizeRange(true)
{
}
//----------------------------------------------------------------------------
void UIFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	SizeNode::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mBackgroundPicBox);

	source.ReadString(mUIScriptHandler);

	source.ReadPointer(mIPTCtrl);
	source.ReadPointer(mGridAlignCtrl);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UIFrame::Link(InStream& source)
{
	SizeNode::Link(source);

	if (mBackgroundPicBox)
		source.ResolveLink(mBackgroundPicBox);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);

	if (mGridAlignCtrl)
		source.ResolveLink(mGridAlignCtrl);
}
//----------------------------------------------------------------------------
void UIFrame::PostLink()
{
	SizeNode::PostLink();
}
//----------------------------------------------------------------------------
bool UIFrame::Register(OutStream& target) const
{
	if (SizeNode::Register(target))
	{
		if (mBackgroundPicBox)
			target.Register(mBackgroundPicBox);

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

	target.WritePointer(mBackgroundPicBox);

	target.WriteString(mUIScriptHandler);

	target.WritePointer(mIPTCtrl);
	target.WritePointer(mGridAlignCtrl);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UIFrame::GetStreamingSize(Stream &stream) const
{
	int size = SizeNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mBackgroundPicBox);

	size += PX2_STRINGSIZE(mUIScriptHandler);
	
	size += PX2_POINTERSIZE(mIPTCtrl);
	size += PX2_POINTERSIZE(mGridAlignCtrl);

	return size;
}
//----------------------------------------------------------------------------