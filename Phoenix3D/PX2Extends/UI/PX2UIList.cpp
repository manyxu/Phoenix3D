// PX2UIList.cpp

#include "PX2UIList.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIList);
PX2_IMPLEMENT_STREAM(UIList);
PX2_IMPLEMENT_FACTORY(UIList);

//----------------------------------------------------------------------------
UIList::UIList() :
mIsNeedRecal(false),
mItemHeight(20.0f)
{
}
//----------------------------------------------------------------------------
UIList::~UIList()
{
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
	AttachChild(item);
	mItems.push_back(item);

	item->GetText()->SetText(text);
	item->GetText()->SetAutoWarp(true);

	return item;
}
//----------------------------------------------------------------------------
void UIList::OnChildAdded(Movable *child)
{
	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
	}
}
//----------------------------------------------------------------------------
void UIList::OnChildRemoved(Movable *child)
{
	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
	}
}
//----------------------------------------------------------------------------
void UIList::OnSizeChanged()
{
	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
void UIList::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if (mIsNeedRecal)
	{
		_Recal();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIList::_Recal()
{
	int itemIndex = 0;
	for (int i = 0; i < GetNumChildren(); i++)
	{
		UIItem *item = DynamicCast<UIItem>(GetChild(i));

		if (item)
		{
			item->GetText()->SetRect(Rectf(-mSize.Width/2.0f,
				-mSize.Height/2.0f, mSize.Width/2.0f, mSize.Height/2.0f));
			item->SetAnchorHor(0.0f, 1.0f);
			item->SetAnchorVer(1.0f, 1.0f);
			item->SetAnchorParamVer(
				-mItemHeight*0.5f - mItemHeight*itemIndex, 0.0f);

			itemIndex++;
		}
	}

	mIsNeedRecal = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIList::UIList(LoadConstructor value) :
UIFrame(value),
mIsNeedRecal(false),
mItemHeight(20.0f)
{
}
//----------------------------------------------------------------------------
void UIList::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

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

	PX2_END_DEBUG_STREAM_SAVE(UIList, target);
}
//----------------------------------------------------------------------------
int UIList::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------