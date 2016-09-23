// PX2UIComboBox.cpp

#include "PX2UIComboBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIComboBox);
PX2_IMPLEMENT_STREAM(UIComboBox);
PX2_IMPLEMENT_FACTORY(UIComboBox);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIComboBox);


//----------------------------------------------------------------------------
UIComboBox::UIComboBox() :
mChoose(-1)
{
	SetSize(100.0f, 40.0f);

	mSelectButton = new0 UIButton();
	AttachChild(mSelectButton);
	mSelectButton->LocalTransform.SetTranslateY(-1.0f);
	mSelectButton->SetAnchorHor(0.0f, 1.0f);
	mSelectButton->SetAnchorVer(0.0f, 1.0f);
	mSelectButton->CreateAddText();
	mSelectButton->SetMemUICallback(this, 
		(UIFrame::MemUICallback)(&UIComboBox::_SelectButCallback));

	mChooseList = new0 UIList();
	AttachChild(mChooseList);
	mChooseList->LocalTransform.SetTranslateY(-3.0f);
	mChooseList->SetAnchorHor(0.0f, 1.0f);
	mChooseList->SetAnchorVer(0.0f, 0.0f);
	mChooseList->SetPivot(0.5f, 1.0f);
	mChooseList->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UIComboBox::_ComboCallback));
	mChooseList->CreateAddBackgroundPicBox();

	mChooseList->Show(false);

	mChooseList->SetUIChildPickOnlyInSizeRange(false);
	SetUIChildPickOnlyInSizeRange(false);
}
//----------------------------------------------------------------------------
UIComboBox::~UIComboBox()
{
}
//----------------------------------------------------------------------------
UIItem *UIComboBox::AddChooseStr(const std::string &choose)
{
	mChooses.push_back(choose);
	UIItem *item = mChooseList->AddItem(choose);
	item->SetUserData("UIComboBoxItemIndex", (int)(mChooses.size() - 1));

	return item;
}
//----------------------------------------------------------------------------
void UIComboBox::RemoveAllChooseStr()
{
	mChooses.clear();
	mChooseList->RemoveAllItems();
	mSelectButton->GetText()->SetText("");
}
//----------------------------------------------------------------------------
int UIComboBox::GetNumChooseStr() const
{
	return (int)mChooses.size();
}
//----------------------------------------------------------------------------
std::string UIComboBox::GetChooseStrByIndex(int i) const
{
	if (0 <= i && i < (int)mChooses.size())
	{
		return mChooses[i];
	}

	return "";
}
//----------------------------------------------------------------------------
void UIComboBox::Choose(int i)
{
	mChoose = i;
	mChooseList->Show(false);

	UIText *text = mSelectButton->GetText();
	text->SetText(GetChooseStrByIndex(mChoose));

	OnChoosed();
}
//----------------------------------------------------------------------------
std::string UIComboBox::GetChooseStr() const
{
	return GetChooseStrByIndex(mChoose);
}
//----------------------------------------------------------------------------
void UIComboBox::OnChoosed()
{
	_UICallbacksCalls(UICT_COMBOBOX_CHOOSED);

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_COMBOBOX_CHOOSED);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_COMBOBOX_CHOOSED);
	}
}
//----------------------------------------------------------------------------
void UIComboBox::_SelectButCallback(UIFrame *frame, UICallType type)
{
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but == mSelectButton)
	{
		if (UICT_RELEASED == type)
		{
			if (IsEnable())
			{
				mChooseList->Show(!mChooseList->IsShow());
			}
		}
		else if (UICT_RELEASED_NOTPICK == type)
		{
			if (IsEnable())
			{
				mChooseList->Show(false);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIComboBox::_ComboCallback(UIFrame *frame, UICallType type)
{
	UIList *list = DynamicCast<UIList>(frame);
	if (list)
	{
		if (UICT_LIST_SELECTED == type)
		{
			int index = list->GetSelectIndex();
			Choose(index);
		}
	}
}
//----------------------------------------------------------------------------
void UIComboBox::SetTextColor(const Float3 &textColor)
{
	if (mSelectButton)
	{
		mSelectButton->GetText()->SetFontColor(textColor);
	}

	if (mChooseList)
	{
		mChooseList->SetTextColor(textColor);
	}
}
//----------------------------------------------------------------------------
void UIComboBox::SetChooseListHeightSameWithChooses()
{
	mChooseList->SetHeight(mChooseList->GetContentHeight());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIComboBox::UIComboBox(LoadConstructor value) :
UIFrame(value),
mChoose(-1)
{
}
//----------------------------------------------------------------------------
void UIComboBox::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIComboBox, source);
}
//----------------------------------------------------------------------------
void UIComboBox::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIComboBox::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIComboBox::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIComboBox::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIComboBox, target);
}
//----------------------------------------------------------------------------
int UIComboBox::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------