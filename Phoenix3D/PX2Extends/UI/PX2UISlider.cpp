// PX2UISlider.cpp

#include "PX2UISlider.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UICanvas.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISlider);
PX2_IMPLEMENT_STREAM(UISlider);
PX2_IMPLEMENT_FACTORY(UISlider);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UISlider);

//----------------------------------------------------------------------------
UISlider::UISlider() :
mButSliderWidth(20.0f),
mPercent(0.0f),
mIsNeededUpdate(true),
mIsDraging(false)
{
	SetName("UISlider");

	mDirectionType = DT_HORIZONTAL;
	SetSize(200.0f, 10.0f);
	SetPivot(0.5f, 0.5f);

	EnableAnchorLayout(false);

	_GenSlider();
}
//----------------------------------------------------------------------------
UISlider::~UISlider()
{
}
//----------------------------------------------------------------------------
void UISlider::SetDirectionType(DirectionType dt)
{
	mDirectionType = dt;
}
//----------------------------------------------------------------------------
void UISlider::SetPercent(float percent)
{
	mPercent = percent;

	mIsNeededUpdate = true;

	if (mUICallback)
	{
		mUICallback(this, UICT_SLIDERCHANGED);
	}

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_SLIDERCHANGED);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_SLIDERCHANGED);
	}

	if (!mUIScriptHandler.empty())
	{
		CallString(mUIScriptHandler.c_str(), "i", (int)UICT_SLIDERCHANGED);
	}
}
//----------------------------------------------------------------------------
void UISlider::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNeededUpdate)
		_UpdateSlider();

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UISlider::_GenSlider()
{
	mFPicBoxBack = new0 UIFPicBox();
	AttachChild(mFPicBoxBack);
	mFPicBoxBack->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mFPicBoxBack->SetAnchorHor(0.0f, 1.0f);
	mFPicBoxBack->SetAnchorVer(0.0f, 1.0f);

	mButSlider = new0 UIButton();
	AttachChild(mButSlider);
	mButSlider->SetSize(Sizef(mButSliderWidth, 0.0f));
	mButSlider->SetAnchorHor(0.0f, 0.0f);
	mButSlider->SetAnchorParamHor(mButSliderWidth/2.0f, 0.0f);
	mButSlider->SetAnchorVer(0.0f, 1.0f);
	UIPicBox *picBoxNormal = mButSlider->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
	picBoxNormal->SetTexture("Data/engine/white.png");
	picBoxNormal->SetColor(Float3::MakeColor(150, 150, 150));
	UIPicBox *picBoxHovered = mButSlider->GetPicBoxAtState(UIButtonBase::BS_HOVERED);
	picBoxHovered->SetTexture("Data/engine/white.png");
	picBoxHovered->SetColor(Float3::MakeColor(175, 175, 175));
	UIPicBox *picBoxPressed = mButSlider->GetPicBoxAtState(UIButtonBase::BS_PRESSED);
	picBoxPressed->SetTexture("Data/engine/white.png");
	picBoxPressed->SetColor(Float3::MakeColor(200, 200, 200));

	mButSlider->SetMemUICallback(this, (UIFrame::MemUICallback)(&UISlider::_SliderDrag));
}
//----------------------------------------------------------------------------
void UISlider::_SetDraging(bool draging)
{
	mIsDraging = draging;
}
//----------------------------------------------------------------------------
void UISlider::_SliderDrag(UIFrame *frame, UICallType type)
{
	UICanvas *uiCanvas = DynamicCast<UICanvas>(GetFirstParentDerivedFromType(UICanvas::TYPE));
	if (!uiCanvas) return;

	if (UICT_PRESSED == type)
	{
		if (frame == mButSlider)
		{
			_SetDraging(true);

			mDragStartPos = uiCanvas->GetCurPickPos();
		}
	}
	else if (UICT_RELEASED == type)
	{
		if (frame == mButSlider)
		{
			_SetDraging(false);
		}
	}
}
//----------------------------------------------------------------------------
void UISlider::OnUIPicked(int info, Movable *child)
{
	UICanvas *uiCanvas = DynamicCast<UICanvas>(GetFirstParentDerivedFromType(UICanvas::TYPE));
	if (!uiCanvas) return;

	if (UIPT_MOVED == info)
	{
		if (IsDraging())
		{
			const Sizef &size = GetSize();
			APoint curPos = uiCanvas->GetCurPickPos();
			AVector moveDir = curPos - mDragStartPos;
			
			if (moveDir.X() > 0.0f)
			{
				float percent = moveDir.X()/(size.Width - mButSliderWidth);
				if (percent > 1.0f) percent = 1.0f;
				SetPercent(percent);
			}
			else
			{
				SetPercent(0.0f);
			}
		}
	}

	UIFrame::OnUIPicked(info, child);
}
//----------------------------------------------------------------------------
void UISlider::OnUINotPicked(int info)
{
	if (UIPT_MOVED == info)
	{

	}

	UIFrame::OnUINotPicked(info);
}
//----------------------------------------------------------------------------
void UISlider::_UpdateSlider()
{
	const Sizef &size = GetSize();

	float paramHor = mButSliderWidth / 2.0f +
		mPercent*(size.Width - mButSliderWidth);
	mButSlider->SetAnchorParamHor(paramHor, 0.0f);

	mIsNeededUpdate = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UISlider::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UISlider");
}
//----------------------------------------------------------------------------
void UISlider::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISlider::UISlider(LoadConstructor value) :
UIFrame(value),
mPercent(0.0f),
mIsNeededUpdate(true),
mIsDraging(false)
{
}
//----------------------------------------------------------------------------
void UISlider::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UISlider, source);
}
//----------------------------------------------------------------------------
void UISlider::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UISlider::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISlider::Register(OutStream& target) const
{
	return UIFrame::Register(target);
}
//----------------------------------------------------------------------------
void UISlider::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UISlider, target);
}
//----------------------------------------------------------------------------
int UISlider::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
