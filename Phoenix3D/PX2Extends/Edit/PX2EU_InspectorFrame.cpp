// EU_InspectorFrame.cpp

#include "PX2EU_InspectorFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_InspectorFrame);
PX2_IMPLEMENT_STREAM(EU_InspectorFrame);
PX2_IMPLEMENT_FACTORY(EU_InspectorFrame);

//----------------------------------------------------------------------------
float EU_InspectorFrame::sItemSize = 20.0f;
//----------------------------------------------------------------------------
EU_InspectorFrame::EU_InspectorFrame()
{
}
//----------------------------------------------------------------------------
EU_InspectorFrame::~EU_InspectorFrame()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_InspectorFrame::EU_InspectorFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_InspectorFrame, source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_InspectorFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_InspectorFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_InspectorFrame, target);
}
//----------------------------------------------------------------------------
int EU_InspectorFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------