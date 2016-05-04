// PX2PropertyGrid.cpp

#include "PX2UIPropertyGrid.hpp"
#include "PX2Transform.hpp"
#include "PX2UIComboBox.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIPropertyItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UITree, UIPropertyGrid);
PX2_IMPLEMENT_STREAM(UIPropertyGrid);
PX2_IMPLEMENT_FACTORY(UIPropertyGrid);
PX2_IMPLEMENT_DEFAULT_NAMES(UITree, UIPropertyGrid);

//----------------------------------------------------------------------------
UIPropertyGrid::UIPropertyGrid() 
{
	mSpliterFrame = new0 UISplitterFrame(false);
	mMaskFrame->AttachChild(mSpliterFrame);
	mSpliterFrame->SetSize(3.0f, 0.0f);
	mSpliterFrame->LocalTransform.SetTranslateY(-5.0f);
	mSpliterFrame->SetAnchorHor(0.5f, 0.5f);
	mSpliterFrame->SetAnchorParamHor(0.0f, 0.0f);
	mSpliterFrame->SetAnchorVer(0.0f, 1.0f);
	mSpliterFrame->SetAnchorParamVer(0.0f, 0.0f);
	mSpliterFrame->SetDragType(UISplitterFrame::DT_PERCENT);
	mSpliterFrame->Show(false);

	mSpliterFrame->SetMemUICallback(this, 
		(UIFrame::MemUICallback)(&UIPropertyGrid::_SpliterDragingCallback));

	NotFreeObjectPool<UIPropertyItem> *classPool = 
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataClass = UIPropertyItem::IT_CLASS;
	classPool->AllocAllObjects(10, initDataClass);
	mItemPools[UIPropertyItem::IT_CLASS] = classPool;

	NotFreeObjectPool<UIPropertyItem> *comboxPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataComboBox = UIPropertyItem::IT_COMBOX;
	comboxPool->AllocAllObjects(45, initDataComboBox);
	mItemPools[UIPropertyItem::IT_COMBOX] = comboxPool;

	NotFreeObjectPool<UIPropertyItem> *editBoxPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBox = UIPropertyItem::IT_EDITBOX;
	editBoxPool->AllocAllObjects(60, initDataEditBox);
	mItemPools[UIPropertyItem::IT_EDITBOX] = editBoxPool;

	NotFreeObjectPool<UIPropertyItem> *editBox3Pool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataEditBox3 = UIPropertyItem::IT_EDITBOX3;
	editBox3Pool->AllocAllObjects(35, initDataEditBox3);
	mItemPools[UIPropertyItem::IT_EDITBOX3] = editBox3Pool;

	NotFreeObjectPool<UIPropertyItem> *transformPool =
		new0 NotFreeObjectPool<UIPropertyItem>();
	Any initDataTransform = UIPropertyItem::IT_TRANSFORM;
	transformPool->AllocAllObjects(5, initDataTransform);
	mItemPools[UIPropertyItem::IT_TRANSFORM] = transformPool;
}
//----------------------------------------------------------------------------
UIPropertyGrid::~UIPropertyGrid()
{
	std::map<UIPropertyItem::ItemType, NotFreeObjectPool<UIPropertyItem> *>
		::iterator it = mItemPools.begin();
	for (; it != mItemPools.end(); it++)
	{
		delete0(it->second);
	}
	mItemPools.clear();
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SpliterDragingCallback(UIFrame *frame, UICallType type)
{
	if (UICT_SPLITTER_DRAGING == type)
	{
		UISplitterFrame *spFrame = DynamicCast<UISplitterFrame>(frame);
		if (spFrame)
		{	
			for (int i = 0; i < (int)mEditObjs.size(); i++)
			{
				_SetEditObjPos(mEditObjs[i]);
			}

			for (int i = 0; i < (int)mAllItems.size(); i++)
			{
				_SetItemPos(mAllItems[i]);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_AddEditObj(UIFrame *obj)
{
	mEditObjs.push_back(obj);

	_SetEditObjPos(obj);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SetItemPos(UIPropertyItem *item)
{
	float anchorHor = mSpliterFrame->GetAnchorHor()[0];

	UIPropertyItem::ItemType it = item->GetItemType();
	if (it != UIPropertyItem::IT_CLASS)
	{
		item->GetFText()->SetAnchorHor(0.0f, anchorHor);
		item->GetFText()->SetAnchorParamHor(0.0f, -mSpliterFrame->GetSize().Width / 2.0f);
		item->GetFText()->SetAnchorVer(0.0f, 1.0f);
		item->GetFText()->SetAnchorParamVer(0.0f, 0.0f);
	}
}
//----------------------------------------------------------------------------
void UIPropertyGrid::_SetEditObjPos(UIFrame *frame)
{
	float anchorHor = mSpliterFrame->GetAnchorHor()[0];

	frame->SetAnchorHor(anchorHor, 1.0f);
	frame->SetAnchorParamHor(mSpliterFrame->GetSize().Width/2.0f, 0.0f);
	frame->SetAnchorVer(0.0f, 1.0f);
	frame->SetAnchorParamVer(0.5f, -0.5f);
}
//----------------------------------------------------------------------------
UIPropertyItem::ItemType UIPropertyGrid::_GetPropertyItemType(
	Object::PropertyType pt)
{
	if (Object::PT_CLASS == pt)
	{
		return UIPropertyItem::ItemType::IT_CLASS;
	}
	else if (Object::PT_INT == pt || PT_FLOAT==pt)
	{
		return UIPropertyItem::ItemType::IT_EDITBOX;
	}
	else if (Object::PT_BOOL == pt || Object::PT_ENUM == pt)
	{
		return UIPropertyItem::ItemType::IT_COMBOX;
	}
	else if (Object::PT_BOOLCHECK == pt)
	{
	}
	else if (Object::PT_FLOAT2 == pt || PT_SIZE == pt)
	{
		return UIPropertyItem::ItemType::IT_EDITBOX2;
	}
	else if (Object::PT_FLOAT3 == pt || Object::PT_FLOAT4 == pt ||
		Object::PT_APOINT3 == pt || Object::PT_APOINT4 == pt ||
		Object::PT_AVECTOR3 == pt || Object::PT_AVECTOR4 == pt ||
		Object::PT_COLOR3FLOAT3 == pt)
	{
		return UIPropertyItem::ItemType::IT_EDITBOX3;
	}
	else if (Object::PT_RECT == pt)
	{
		return UIPropertyItem::ItemType::IT_EDITBOX4;
	}
	else if (Object::PT_STRING == pt || PT_STRINGBUTTON==pt)
	{
		return UIPropertyItem::ItemType::IT_EDITBOX;
	}
	else if (Object::PT_TRANSFORM == pt)
	{
		return UIPropertyItem::ItemType::IT_TRANSFORM;
	}

	return UIPropertyItem::ItemType::IT_NONE;
}
//----------------------------------------------------------------------------
NotFreeObjectPool<UIPropertyItem> *UIPropertyGrid::_GetPool(
	UIPropertyItem::ItemType itemType)
{
	std::map<UIPropertyItem::ItemType, NotFreeObjectPool<UIPropertyItem> *>
		::iterator it = mItemPools.find(itemType);
	if (it == mItemPools.end())
		return 0;

	return it->second;
}
//----------------------------------------------------------------------------
UIPropertyItem *UIPropertyGrid::AddItemType(UIItem *parentItem,
	const std::string &label, Object::PropertyType pt,
	const std::string &name, const Any &data, Object *obj)
{
	mSpliterFrame->Show(true);

	// get pool
	UIPropertyItem::ItemType itemType = _GetPropertyItemType(pt);
	NotFreeObjectPool<UIPropertyItem> *pool = _GetPool(itemType);
	if (!pool) return 0;

	// new item
	UIPropertyItem *item = pool->AllocObject();

	parentItem->AttachChild(item);

	item->SetName(name);
	item->SetSize(parentItem->GetSize());
	item->GetFText()->GetText()->SetText(label);
	item->SetItemObject(obj);
	item->GetFText()->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
	item->SetItemType(itemType, pt, data);

	UIFrame *editCtrl = item->GetEditCtrl();
	if (editCtrl)
	{
		_AddEditObj(editCtrl);
	}

	mAllItems.push_back(item);
	_SetItemPos(item);

	// but back
	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UITree::SelectCallback));
	}

	if (!name.empty())
	{
		mTagItems[name] = item;
	}

	return item;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::RemoveAllProperties()
{
	mSpliterFrame->Show(false);

	for (int i = 0; i < (int)mAllItems.size(); i++)
	{
		UIPropertyItem *propertyItem = mAllItems[i];
		if (propertyItem != mRootItem)
		{
			NotFreeObjectPool<UIPropertyItem> *pool = _GetPool(
				propertyItem->GetItemType());
			if (pool)
			{
				pool->FreeObject(propertyItem->GetObjectID());
			}
		}
	}
	mAllItems.clear();

	mRootItem->RemoveAllChildItems();
	mLastClassItem = 0;

	mTagItems.clear();
	mEditObjs.clear();
}
//----------------------------------------------------------------------------
UIItem *UIPropertyGrid::AddProperty(const std::string &name,
	const std::string &label, PropertyType type, const Any &data, bool enable)
{
	UIPropertyItem *item = 0;
	if (PT_CLASS == type)
	{
		item = AddItemType(mRootItem, label, type, name, data);
		mLastClassItem = item;
	}
	else
	{
		if (PT_INT == type || PT_BOOL == type || PT_FLOAT == type || 
			PT_FLOAT2 == type || PT_FLOAT3 == type || PT_STRING == type ||
			PT_COLOR3FLOAT3 == type || PT_AVECTOR3 == type || PT_APOINT3 == type 
			|| PT_ENUM == type || PT_SIZE==type)
		{
			item = AddItemType(mLastClassItem, label, type, name, data);
		}
		else if (PT_TRANSFORM == type)
		{
			UIItem *transformItem = AddItemType(mLastClassItem, label, type, name, data);

			Transform trans = PX2_ANY_AS(data, Transform);
			const APoint &pos = trans.GetTranslate();
			float rotX = 0.0f; float rotY = 0.0f; float rotZ = 0.0f;
			trans.GetRotate(rotX, rotY, rotZ);
			APoint rot(rotX, rotY, rotZ);
			APoint scale = trans.GetScale();

			AddItemType(transformItem, "Pos", Object::PT_APOINT3, "Pos", pos);
			AddItemType(transformItem, "Rotate", Object::PT_APOINT3, "Rotate", rot);
			AddItemType(transformItem, "Scale", Object::PT_APOINT3, "Scale", scale);
		}
		else
		{
			item = AddItemType(mLastClassItem, "Not_" + label, Object::PT_STRING, name, data);
		}
	}

	return item;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::OnItemChanged(UIItem *item)
{
	PX2_UNUSED(item);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIPropertyGrid::UIPropertyGrid(LoadConstructor value) :
UITree(value)
{
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UITree::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPropertyGrid, source);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Link(InStream& source)
{
	UITree::Link(source);
}
//----------------------------------------------------------------------------
void UIPropertyGrid::PostLink()
{
	UITree::PostLink();
}
//----------------------------------------------------------------------------
bool UIPropertyGrid::Register(OutStream& target) const
{
	if (UITree::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPropertyGrid::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UITree::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIPropertyGrid, target);
}
//----------------------------------------------------------------------------
int UIPropertyGrid::GetStreamingSize(Stream &stream) const
{
	int size = UITree::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------