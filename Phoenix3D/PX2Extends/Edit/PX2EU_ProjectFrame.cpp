// EU_ProjectFrame.cpp

#include "PX2EU_ProjectFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_ProjectFrame);
PX2_IMPLEMENT_STREAM(EU_ProjectFrame);
PX2_IMPLEMENT_FACTORY(EU_ProjectFrame);

//----------------------------------------------------------------------------
float EU_ProjectFrame::sItemSize = 20.0f;
//----------------------------------------------------------------------------
EU_ProjectFrame::EU_ProjectFrame()
{
	mRootItem = new UIItem();
	AttachChild(mRootItem);
	mRootItem->LocalTransform.SetTranslateZ(-1.0f);
	mRootItem->SetSize(10.0f, sItemSize);
	mRootItem->SetAnchorHor(0.0f, 1.0f);
	mRootItem->SetAnchorVer(1.0f, 1.0f);
	mRootItem->SetAnchorParamVer(-sItemSize/2.0f, 0.0f);
	mRootItem->CreateAddBackgroundPicBox();
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