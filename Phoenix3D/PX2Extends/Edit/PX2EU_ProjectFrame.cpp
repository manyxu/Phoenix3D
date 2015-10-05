// EU_ProjectFrame.cpp

#include "PX2EU_ProjectFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ProjectFrame);
PX2_IMPLEMENT_STREAM(EU_ProjectFrame);
PX2_IMPLEMENT_FACTORY(EU_ProjectFrame);

//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame()
{
	mTree = new0 EU_ProjectTree();
	AttachChild(mTree);
	mTree->SetAnchorHor(0.0f, 1.0f);
	mTree->SetAnchorVer(0.0f, 1.0f);

	UIItem *rootItem = mTree->GetRootItem();
	rootItem->AddItem("ABCD");

	UIItem *itemABC = rootItem->AddItem("ABC");
	itemABC->AddItem("abc1");
	itemABC->AddItem("abc2");

	rootItem->AddItem("ABC");

	UIItem *itemAB = rootItem->AddItem("AB");
	itemAB->AddItem("ab1");
	UIItem *itemab2 = itemAB->AddItem("ab2");
	itemab2->AddItem("ab2_1");
	itemab2->AddItem("ab2_2");

	rootItem->AddItem("AA");
	rootItem->AddItem("A");

	rootItem->Expand(true);
}
//----------------------------------------------------------------------------
EU_ProjectFrame::~EU_ProjectFrame()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_ProjectFrame, source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_ProjectFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_ProjectFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_ProjectFrame, target);
}
//----------------------------------------------------------------------------
int EU_ProjectFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------