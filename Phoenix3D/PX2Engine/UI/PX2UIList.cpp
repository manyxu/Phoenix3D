// PX2UIList.cpp

#include "PX2UIList.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIList);
PX2_IMPLEMENT_STREAM(UIList);
PX2_IMPLEMENT_FACTORY(UIList);

//----------------------------------------------------------------------------
UIList::UIList() :
mIsNeedRecal(false),
mIsUpdateSliderVisible(true),
mSliderSize(10),
mItemHeight(20.0f),
mIsUpdateContentPos(true),
mSelectedIndex(-1)
{
	mMaskFrame = new0 UIFrame();
	AttachChild(mMaskFrame);
	mMaskFrame->LocalTransform.SetTranslateY(-1.0f);
	mMaskFrame->CreateAddMask();
	mMaskFrame->SetAnchorHor(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamHor(0.0f, -mSliderSize);
	mMaskFrame->SetAnchorVer(0.0f, 1.0f);

	mContentFrame = new0 UIFrame();
	mMaskFrame->AttachChild(mContentFrame);
	mContentFrame->LocalTransform.SetTranslateY(-1.0f);
	mContentFrame->SetAnchorHor(0.0f, 1.0f);
	mContentFrame->SetAnchorParamHor(0.0f, 0.0f);
	mContentFrame->SetAnchorVer(1.0f, 1.0f);
	mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	mContentFrame->SetPivot(0.5f, 1.0f);

	mSlider = new0 UISlider();
	mSlider->LocalTransform.SetTranslateY(-1.0f);
	mSlider->SetDirectionType(UISlider::DT_VERTICALITY);
	mSlider->EnableAnchorLayout(true);
	mSlider->SetAnchorHor(1.0f, 1.0f);
	mSlider->SetAnchorParamHor(-mSliderSize*0.5f, 0.0f);
	mSlider->SetAnchorVer(0.0f, 1.0f);
	mSlider->SetSize(mSliderSize, 0.0f);
	mSlider->SetPivot(0.5f, 0.5f);
	AttachChild(mSlider);
	mSlider->SetContentFrame(mContentFrame);
	mSlider->SetMemUICallback(this, (UIFrame::MemUICallback)(&UIList
		::_SliderCallback));

	SetUIChildPickOnlyInSizeRange(true);

	mTextColor = Float3::WHITE;
}
//----------------------------------------------------------------------------
UIList::~UIList()
{
}
//----------------------------------------------------------------------------
void UIList::_SliderCallback(UIFrame *frame, UICallType type)
{
	UISlider *slider = DynamicCast<UISlider>(frame);
	if (slider)
	{
		if (UICT_SLIDERCHANGED == type)
		{
			mIsUpdateContentPos = true;
			mIsUpdateSliderVisible = true;
		}
	}
}
//----------------------------------------------------------------------------
void UIList::_UpdateContentPos()
{
	float heightDist = mContentFrame->GetSize().Height - GetSize().Height;
	if (heightDist > 0.0f)
	{
		float downMove = heightDist * mSlider->GetPercent();
		mContentFrame->SetAnchorParamVer(downMove, 0.0f);
	}
	else
	{
		mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	}

	mIsUpdateContentPos = false;
}
//----------------------------------------------------------------------------
void UIList::_UpdateItemVisible()
{
	for (int i = 0; i < mContentFrame->GetNumChildren(); i++)
	{
		UIItem *item = DynamicCast<UIItem>(mContentFrame->GetChild(i));

		if (item)
		{
			item->Show(IsIntersectSizeRange(item));
		}
	}

	mIsUpdateSliderVisible = false;
}
//----------------------------------------------------------------------------
void UIList::_SelectButCallback(UIFrame *frame, UICallType type)
{
	UIButton *button = DynamicCast<UIButton>(frame);
	if (button)
	{
		UIItem *uiItem = DynamicCast<UIItem>(button->GetParent());
		if (uiItem && UICT_PRESSED == type)
		{
			ClearAllSelectItems();
			AddSelectItem(uiItem);

			OnSelected(uiItem);
		}
	}
}
//----------------------------------------------------------------------------
void UIList::SetSliderSize(float size)
{
	mSliderSize = size;
	mSlider->SetSize(mSliderSize, 0.0f);
}
//----------------------------------------------------------------------------
void UIList::SetItemHeight(float height)
{
	mItemHeight = height;

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
UIItem *UIList::AddItem(const std::string &text)
{
	UIItem *item = new0 UIItem();
	mContentFrame->AttachChild(item);
	mItems.push_back(item);

	item->GetFText()->GetText()->SetText(text);
	item->GetFText()->GetText()->SetAutoWarp(true);
	item->GetFText()->GetText()->SetFontColor(mTextColor);

	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UIList::_SelectButCallback));
	}

	item->SetUserData("index", (int)(mItems.size() - 1));

	mIsNeedRecal = true;

	return item;
}
//----------------------------------------------------------------------------
void UIList::RemoveAllItems()
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		mContentFrame->DetachChild(mItems[i]);
	}
	mItems.clear();

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
float UIList::GetContentHeight() const
{
	return mItemHeight * (int)mItems.size();
}
//----------------------------------------------------------------------------
void UIList::AddSelectItem(UIItem *item)
{
	if (item->IsSelected())
		return;

	item->Select(true);

	mSelectedItems.push_back(item);
}
//----------------------------------------------------------------------------
void UIList::ClearAllSelectItems()
{
	for (int i = 0; i < (int)mSelectedItems.size(); i++)
	{
		mSelectedItems[i]->Select(false);
	}

	mSelectedItems.clear();
}
//----------------------------------------------------------------------------
UIItem *UIList::GetSelectedItem()
{
	if ((int)mSelectedItems.size() > 0)
	{
		return mSelectedItems[0];
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIList::OnSelected(UIItem *item)
{
	mSelectedIndex = item->GetUserData<int>("index");

	_UICallbacksCalls(UICT_LIST_SELECTED);

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_LIST_SELECTED);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_LIST_SELECTED);
	}
}
//----------------------------------------------------------------------------
void UIList::SetTextColor(const Float3 &textColor)
{
	mTextColor = textColor;

	for (int i = 0; i < (int)mItems.size(); i++)
	{
		UIItem *item = mItems[i];
		if (item)
		{
			item->GetFText()->GetText()->SetColor(textColor);
		}
	}
}
//----------------------------------------------------------------------------
void UIList::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	mIsNeedRecal = true;
	mIsUpdateSliderVisible = true;
	mIsUpdateContentPos = true;
}
//----------------------------------------------------------------------------
void UIList::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if (mIsNeedRecal)
	{
		_Recal();
	}

	if (mIsUpdateContentPos)
	{
		_UpdateContentPos();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsUpdateSliderVisible)
		_UpdateItemVisible();
}
//----------------------------------------------------------------------------
void UIList::_Recal()
{
	int itemIndex = 0;
	for (int i = 0; i < mContentFrame->GetNumChildren(); i++)
	{
		UIItem *item = DynamicCast<UIItem>(mContentFrame->GetChild(i));

		if (item)
		{
			item->SetAnchorHor(0.0f, 1.0f);
			item->SetAnchorParamHor(0.0f, 0.0f);
			item->SetAnchorVer(1.0f, 1.0f);
			item->SetSize(0.0f, mItemHeight);
			float height = mItemHeight*0.5f + mItemHeight*itemIndex;
			item->SetAnchorParamVer(-height, 0.0f);

			itemIndex++;
		}
	}

	mContentFrame->SetSize(0.0f, GetContentHeight());
	mIsUpdateContentPos = true;

	mIsNeedRecal = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIList::UIList(LoadConstructor value) :
UIFrame(value),
mIsNeedRecal(false),
mIsUpdateSliderVisible(true),
mSliderSize(10),
mItemHeight(20.0f),
mSelectedIndex(-1)
{
	SetUIChildPickOnlyInSizeRange(true);
}
//----------------------------------------------------------------------------
void UIList::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mTextColor);

	PX2_END_DEBUG_STREAM_LOAD(UIList, source);
}
//----------------------------------------------------------------------------
void UIList::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIList::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIList::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIList::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mTextColor);

	PX2_END_DEBUG_STREAM_SAVE(UIList, target);
}
//----------------------------------------------------------------------------
int UIList::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mTextColor);

	return size;
}
//----------------------------------------------------------------------------