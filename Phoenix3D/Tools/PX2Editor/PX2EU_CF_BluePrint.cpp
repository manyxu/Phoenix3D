// PX2EU_CF_BluePrint.cpp

#include "PX2EU_CF_BluePrint.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_CF_BluePrint);
PX2_IMPLEMENT_STREAM(EU_CF_BluePrint);
PX2_IMPLEMENT_FACTORY(EU_CF_BluePrint);

//----------------------------------------------------------------------------
EU_CF_BluePrint::EU_CF_BluePrint()
{
	LocalTransform.SetTranslateY(-5.0f);

	mTree = new0 UITree();
	AttachChild(mTree);
	mTree->ShowRootItem(false);
	mTree->SetAnchorHor(0.0f, 1.0f);
	mTree->SetAnchorVer(0.0f, 1.0f);

	UIItem *objectItem = mTree->AddItem(mTree->GetRootItem(), "Object");
	mTree->AddItem(objectItem, "GetName");
	mTree->AddItem(objectItem, "SetID");
	mTree->AddItem(objectItem, "GetID");
	UIItem *timeItem = mTree->AddItem(mTree->GetRootItem(), "Time");
	UIItem *serialItem = mTree->AddItem(mTree->GetRootItem(), "Serial");
}
//----------------------------------------------------------------------------
EU_CF_BluePrint::~EU_CF_BluePrint()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_CF_BluePrint::EU_CF_BluePrint(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_CF_BluePrint::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_CF_BluePrint, source);
}
//----------------------------------------------------------------------------
void EU_CF_BluePrint::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_CF_BluePrint::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_CF_BluePrint::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_CF_BluePrint::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_CF_BluePrint, target);
}
//----------------------------------------------------------------------------
int EU_CF_BluePrint::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------