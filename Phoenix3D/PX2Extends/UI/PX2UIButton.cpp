// PX2UIButton.cpp

#include "PX2UIButton.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UICanvas.hpp"
#include "PX2FunObject.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIEvent.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIButtonBase, UIButton);
PX2_IMPLEMENT_STREAM(UIButton);
PX2_IMPLEMENT_FACTORY(UIButton);
PX2_IMPLEMENT_DEFAULT_NAMES(UIButtonBase, UIButton);

//----------------------------------------------------------------------------
UIButton::UIButton() :
mAfterReleasedType(ART_NORMAL),
mIsRecoverBegin(false),
mAfterReleasedRecoverTime(6.0f),
mRecoverBeginTime(0.0f)
{
	SetName("UIButton");

	if (BT_COLOR == mButType)
	{
		UIPicBox *picNormal = new0 UIPicBox("Data/engine/white.png");
		picNormal->SetName("Normal");
		SetPicBox(BS_NORMAL, picNormal);
	}
	else
	{
		UIPicBox *picNormal = new0 UIPicBox("Data/engine/white.png");
		picNormal->SetName("Normal");
		SetPicBox(BS_NORMAL, picNormal);

		UIPicBox *picOver = new0 UIPicBox("Data/engine/white.png");
		picOver->SetName("Over");
		SetPicBox(BS_HOVERED, picOver);

		UIPicBox *picDown = new0 UIPicBox("Data/engine/white.png");
		picDown->SetName("Down");
		SetPicBox(BS_PRESSED, picDown);

		UIPicBox *picDis = new0 UIPicBox("Data/engine/white.png");
		picDis->SetName("Dis");
		SetPicBox(BS_DISABLED, picDis);
	}

	SetButtonState(BS_NORMAL);

	SetSize(40.0f, 40.0f);
}
//----------------------------------------------------------------------------
UIButton::~UIButton()
{
}
//----------------------------------------------------------------------------
void UIButton::Enable(bool enable)
{
	UIButtonBase::Enable(enable);

	SetButtonState(enable ? UIButtonBase::BS_NORMAL:UIButtonBase::BS_DISABLED);
}
//----------------------------------------------------------------------------
void UIButton::SetAfterReleasedType(AfterReleasedType type)
{
	if (type == mAfterReleasedType)
		return;

	Enable(true);
	mAfterReleasedType = type;

	if (ART_NORMAL == mAfterReleasedType ||
		ART_DISABLE == mAfterReleasedType)
	{
		mAfterReleasedRecoverTime = 0.0f;
		mIsRecoverBegin = false;
		mRecoverBeginTime = (float)Time::GetTimeInSeconds();
	}
}
//----------------------------------------------------------------------------
void UIButton::OnEvent(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void UIButton::OnPressed()
{
	if (mUICallback)
	{
		mUICallback(this, UICT_PRESSED);
	}

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_PRESSED);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_PRESSED);
	}
}
//----------------------------------------------------------------------------
void UIButton::OnPressedNotPick()
{
	if (mUICallback)
	{
		mUICallback(this, UICT_PRESSED_NOTPICK);
	}

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_PRESSED_NOTPICK);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_PRESSED_NOTPICK);
	}
}
//----------------------------------------------------------------------------
void UIButton::OnReleased()
{
	if (ART_DISABLE == mAfterReleasedType)
	{
		Enable(false);
	}
	else if (ART_DISABLE_RECOVER == mAfterReleasedType)
	{
		Enable(false);
		mIsRecoverBegin = true;
		mRecoverBeginTime = (float)Time::GetTimeInSeconds();
	}

	if (mUICallback)
	{
		mUICallback(this, UICT_RELEASED);
	}
	
	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_RELEASED);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_RELEASED);
	}

	Event *ent = UIES::CreateEventX(UIES::ButReleased);
	ent->SetData<UIButton*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void UIButton::OnReleasedNotPick()
{
	if (mUICallback)
	{
		mUICallback(this, UICT_RELEASED_NOTPICK);
	}

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_RELEASED_NOTPICK);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_RELEASED_NOTPICK);
	}
}
//----------------------------------------------------------------------------
void UIButton::SetActivate(bool act)
{
	UIButtonBase::SetActivate(act);

	ButType bt = GetButType();
	if (BT_COLOR == bt)
	{
		SetButtonState(GetButtonState());
	}
	else
	{
	}
}
//----------------------------------------------------------------------------
void UIButton::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIButtonBase::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsRecoverBegin)
	{
		float curTime = (float)Time::GetTimeInSeconds();

		if ((curTime - mRecoverBeginTime) > mAfterReleasedRecoverTime)
		{
			Enable(true);

			mIsRecoverBegin = false;
		}
	}
}
//----------------------------------------------------------------------------
void UIButton::OnUIPicked(const UIInputData &inputData)
{
	ButtonState state = GetButtonState();

	if (UIPT_PRESSED == inputData.PickType)
	{
		if (state == BS_NORMAL || state == BS_HOVERED)
		{
			SetButtonState(BS_PRESSED);
			OnPressed();
		}
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		if (state == BS_PRESSED)
		{
			SetButtonState(BS_HOVERED);
			OnReleased();
		}
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (state == BS_NORMAL)
		{
			SetButtonState(BS_HOVERED);
		}
	}
}
//----------------------------------------------------------------------------
void UIButton::OnUINotPicked(const UIInputData &inputData)
{
	ButtonState state = GetButtonState();

	if (UICT_PRESSED == inputData.PickType)
	{
		OnPressedNotPick();
	}
	else if (UICT_RELEASED == inputData.PickType)
	{
		if (BS_PRESSED == state)
		{
			SetButtonState(BS_NORMAL);
		}

		OnReleasedNotPick();
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (state == BS_HOVERED)
		{
			SetButtonState(BS_NORMAL);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// UIButton
//----------------------------------------------------------------------------
FunObject *UIButton::RegistClassFunctions()
{
	FunObject *parentFunObj = UIButtonBase::RegistClassFunctions();

	FunObject *thisFunObj = parentFunObj->GetAddClass("UIButton");

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "New";
		funObj->AddInput("static", FPT_POINTER_THIS_STATIC, (Object*)0);
		funObj->AddOutput("ot_but", FPT_POINTER, (Object*)0);
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetOnPressedFun";
		funObj->AddInput("in_but", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_funstr", FPT_STRING, std::string(""));
		thisFunObj->AddFunObject(funObj);
	}

	{
		FunObjectPtr funObj = new0 FunObject();
		funObj->FunName = "SetOnReleasedFun";
		funObj->AddInput("in_but", FPT_POINTER_THIS, (Object*)0);
		funObj->AddInput("in_funstr", FPT_STRING, std::string(""));
		thisFunObj->AddFunObject(funObj);
	}

	return thisFunObj;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIButton::RegistProperties()
{
	UIButtonBase::RegistProperties();

	AddPropertyClass("UIButton");

	std::vector<std::string> afterReleasedTypes;
	afterReleasedTypes.push_back("ART_NORMAL");
	afterReleasedTypes.push_back("ART_DISABLE");
	afterReleasedTypes.push_back("ART_DISABLE_RECOVER");
	AddPropertyEnum("AfterReleasedType", mAfterReleasedType,
		afterReleasedTypes);
	AddProperty("AfterReleasedRecoverTime", PT_FLOAT,
		GetAfterReleasedRecoverTime());
}
//----------------------------------------------------------------------------
void UIButton::OnPropertyChanged(const PropertyObject &obj)
{
	UIButtonBase::OnPropertyChanged(obj);

	if ("AfterReleasedType" == obj.Name)
	{
		SetAfterReleasedType((AfterReleasedType)*Any_Cast<int>(&obj.Data));
	}
	else if ("AfterReleasedRecoverTime" == obj.Name)
	{
		SetAfterReleasedRecoverTime(*Any_Cast<float>(&obj.Data));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIButton::UIButton(LoadConstructor value) :
UIButtonBase(value),
mAfterReleasedType(ART_NORMAL),
mAfterReleasedRecoverTime(6.0),
mIsRecoverBegin(false),
mRecoverBeginTime(0.0f)
{
}
//----------------------------------------------------------------------------
void UIButton::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIButtonBase::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mAfterReleasedType);
	source.Read(mAfterReleasedRecoverTime);

	PX2_END_DEBUG_STREAM_LOAD(UIButton, source);
}
//----------------------------------------------------------------------------
void UIButton::Link(InStream& source)
{
	UIButtonBase::Link(source);
}
//----------------------------------------------------------------------------
void UIButton::PostLink()
{
	UIButtonBase::PostLink();
}
//----------------------------------------------------------------------------
bool UIButton::Register(OutStream& target) const
{
	return UIButtonBase::Register(target);
}
//----------------------------------------------------------------------------
void UIButton::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIButtonBase::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mAfterReleasedType);
	target.Write(mAfterReleasedRecoverTime);

	PX2_END_DEBUG_STREAM_SAVE(UIButton, target);
}
//----------------------------------------------------------------------------
int UIButton::GetStreamingSize(Stream &stream) const
{
	int size = UIButtonBase::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mAfterReleasedType);
	size += sizeof(mAfterReleasedRecoverTime);

	return size;
}
//----------------------------------------------------------------------------
