// PX2UIItem.cpp

#include "PX2UIItem.hpp"
#include "PX2UITree.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIItem);
PX2_IMPLEMENT_STREAM(UIItem);
PX2_IMPLEMENT_FACTORY(UIItem);

//----------------------------------------------------------------------------
UIItem::UIItem() :
mIsNeedRecal(true),
mIsExpand(true),
mIsNumAllChildExpandNeedRecal(true),
mNumAllChildExpand(0),
mLevel(0),
mIsShowRootItem(true)
{
	mButBack = new0 UIButton();
	AttachChild(mButBack);
	mButBack->LocalTransform.SetTranslateY(-5.0f);
	UIPicBox *picBoxNormal = mButBack->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
	picBoxNormal->SetTexture("Data/engine/white.png");
	picBoxNormal->SetColor(PX2_UISM.Color_ViewBackground);
	UIPicBox *picBoxHovered = mButBack->GetPicBoxAtState(UIButtonBase::BS_HOVERED);
	picBoxHovered->SetTexture("Data/engine/white.png");
	picBoxHovered->SetColor(PX2_UISM.Color_AuiButTab_Horvered);
	UIPicBox *picBoxPressed = mButBack->GetPicBoxAtState(UIButtonBase::BS_PRESSED);
	picBoxPressed->SetTexture("Data/engine/white.png");
	picBoxPressed->SetColor(PX2_UISM.Color_AuiButTab_Pressed);
	mButBack->SetAnchorHor(0.0f, 1.0f);
	mButBack->SetAnchorVer(0.0f, 1.0f);
	mButBack->SetAnchorParamVer(1.0f, 0.0f);

	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(0.0f, 1.0f);
	mFText->LocalTransform.SetTranslateY(-6.0f);
	mFText->GetText()->SetFontScale(0.6f);
	mFText->GetText()->SetFontColor(Float3::MakeColor(120, 120, 120));
	
	SetPivot(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
UIItem::~UIItem()
{
}
//----------------------------------------------------------------------------
UIItem *UIItem::AddItem(const std::string &label)
{
	UIItem *item = new0 UIItem();
	AttachChild(item);
	item->SetName(label);
	item->SetSize(GetSize());
	item->GetFText()->GetText()->SetText(label);

	return item;
}
//----------------------------------------------------------------------------
void UIItem::OnChildAdded(Movable *child)
{
	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
		mIsNumAllChildExpandNeedRecal = true;

		mChildItems.push_back(item);
	}

	_TellParentChildrenRecal();
}
//----------------------------------------------------------------------------
void UIItem::OnChildRemoved(Movable *child)
{
	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
		mIsNumAllChildExpandNeedRecal = true;
	}

	_TellParentChildrenRecal();
}
//----------------------------------------------------------------------------
bool UIItem::RemoveItem(UIItem *item)
{
	// remove item
	std::vector<Pointer0<UIItem> >::iterator it = mChildItems.begin();
	for (; it != mChildItems.end();)
	{
		if (item == *it)
		{
			it = mChildItems.erase(it);
			DetachChild(item);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void UIItem::RemoveAllChildItems()
{
	for (int i = 0; i < (int)mChildItems.size(); i++)
	{
		DetachChild(mChildItems[i]);
	}

	mChildItems.clear();
}
//----------------------------------------------------------------------------
void UIItem::Expand(bool expand)
{
	mIsExpand = expand;

	if (IAS_NONE != mIconArrowState)
	{
		SetIconArrowState(expand ? IAS_ARROW1 : IAS_ARROW0);
	}

	mIsNumAllChildExpandNeedRecal = true;

	_TellParentChildrenRecal();
}
//----------------------------------------------------------------------------
void UIItem::_TellParentChildrenRecal()
{
	UIItem *parentItem = DynamicCast<UIItem>(GetParent());
	if (parentItem)
	{
		for (int i = 0; i < parentItem->GetNumChildren(); i++)
		{
			UIItem *parentChildItem = DynamicCast<UIItem>(
				parentItem->GetChild(i));
			if (parentChildItem)
			{
				parentChildItem->mIsNeedRecal = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIItem::SetLabel(const std::string &label)
{
	mFText->GetText()->SetText(label);
}
//----------------------------------------------------------------------------
const std::string &UIItem::GetLabel() const
{
	return mFText->GetText()->GetText();
}
//----------------------------------------------------------------------------
void UIItem::OnSizeChanged()
{
	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UIItem::SetIconArrowState(IconArrowState state)
{
	mIconArrowState = state;

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
void UIItem::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNumAllChildExpandNeedRecal)
	{
		_RecalNumChildExpand();
	}

	if (mIsNeedRecal)
	{
		_Recal();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIItem::_RecalNumChildExpand()
{
	mIsNumAllChildExpandNeedRecal = false;

	int numChildren = GetNumChildren();
	for (int i = 0; i < numChildren; i++)
	{
		UIItem *item = DynamicCast<UIItem>(GetChild(i));
		if (item)
		{
			item->_RecalNumChildExpand();
		}
	}

	mNumAllChildExpand = 1;
	if (mIsExpand)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			UIItem *item = DynamicCast<UIItem>(GetChild(i));
			if (item)
			{
				mNumAllChildExpand += item->GetNumAllChildsExpand();
			}
		}
	}
	else
	{
		mNumAllChildExpand = 1;
	}
}
//----------------------------------------------------------------------------
void UIItem::_Recal()
{
	mIsNeedRecal = false;

	int numLevels = 0;
	UITree *tree = DynamicCast<UITree>(GetFirstParentDerivedFromType(
		UITree::TYPE, &numLevels));
	if (tree)
	{
		float iconArrowSpace = tree->GetIconArrowSpace();
		float itemHeight = tree->GetItemHeight();

		float selfVerOffset = -itemHeight / 2.0f;
		if (!mIsShowRootItem)
			selfVerOffset += itemHeight;

		int numItemExtend = 0;
		for (int i = 0; i < GetNumChildren(); i++)
		{
			UIItem *item = DynamicCast<UIItem>(GetChild(i));
			if (item)
			{
				item->SetAnchorHor(0.0f, 1.0f);
				item->SetAnchorVer(1.0f, 1.0f);
				item->_SetLevel(_GetLevel() + 1);

				// -itemHeight/2.0f is this item height
				item->SetAnchorParamVer(
					selfVerOffset - itemHeight * (numItemExtend + 1), 0.0f);
				int itemLevel = item->_GetLevel();
				UIText *text = item->GetFText()->GetText();
				text->SetOffset(Float2(itemLevel*iconArrowSpace, 0.0f));

				item->Show(mIsExpand);

				numItemExtend += item->GetNumAllChildsExpand();
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIItem::_SetLevel(int level)
{
	mLevel = level;
}
//----------------------------------------------------------------------------
void UIItem::_ShowRootItem(bool show)
{
	if (mIsShowRootItem != show)
	{
		mIsShowRootItem = show;

		if (mButBack)
		{
			mButBack->Show(mIsShowRootItem);
		}

		if (mFText)
		{
			mFText->Show(mIsShowRootItem);
		}

		if (mIsShowRootItem)
			mLevel = 0;
		else
			mLevel = -1;
	}

	mIsNeedRecal = true;
	mIsNumAllChildExpandNeedRecal = true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIItem::UIItem(LoadConstructor value) :
UIFrame(value),
mIsNeedRecal(true),
mIsExpand(true),
mIsNumAllChildExpandNeedRecal(true),
mNumAllChildExpand(0),
mLevel(0),
mIsShowRootItem(true)
{
}
//----------------------------------------------------------------------------
void UIItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIItem, source);
}
//----------------------------------------------------------------------------
void UIItem::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UIItem::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIItem::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIItem, target);
}
//----------------------------------------------------------------------------
int UIItem::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------