// PX2UITree.cpp

#include "PX2UITree.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UITree);
PX2_IMPLEMENT_STREAM(UITree);
PX2_IMPLEMENT_FACTORY(UITree);

//----------------------------------------------------------------------------
UITree::UITree() :
mItemHeight(20.0f),
mIconArrowSpace(20.0f)
{
	mRootItem = new UIItem();
	AttachChild(mRootItem);
	mRootItem->LocalTransform.SetTranslateZ(-1.0f);
	mRootItem->SetSize(10.0f, mItemHeight);
	mRootItem->SetAnchorHor(0.0f, 1.0f);
	mRootItem->SetAnchorVer(1.0f, 1.0f);
	mRootItem->SetAnchorParamVer(-mItemHeight / 2.0f, 0.0f);
	mRootItem->SetLabel("RootItem");
}
//----------------------------------------------------------------------------
UITree::~UITree()
{
}
//----------------------------------------------------------------------------
void UITree::SetItemHeight(float height)
{
	mItemHeight = height;
}
//----------------------------------------------------------------------------
UIItem *UITree::AddItem(UIItem *parentItem, const std::string &label)
{
	return parentItem->AddItem(label);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UITree::UITree(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UITree::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UITree, source);
}
//----------------------------------------------------------------------------
void UITree::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mIPTCtrl)
		source.ResolveLink(mIPTCtrl);
}
//----------------------------------------------------------------------------
void UITree::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UITree::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UITree::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UITree, target);
}
//----------------------------------------------------------------------------
int UITree::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------