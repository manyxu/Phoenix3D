// PX2BBBlock.cpp

#include "PX2BBBlock.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, BBBlock);
PX2_IMPLEMENT_STREAM(BBBlock);
PX2_IMPLEMENT_FACTORY(BBBlock);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, BBBlock);

//----------------------------------------------------------------------------
BBBlock::BBBlock()
{

}
//----------------------------------------------------------------------------
BBBlock::~BBBlock()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
BBBlock::BBBlock(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void BBBlock::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(BBBlock, source);
}
//----------------------------------------------------------------------------
void BBBlock::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void BBBlock::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool BBBlock::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void BBBlock::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(BBBlock, target);
}
//----------------------------------------------------------------------------
int BBBlock::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------