// PX2UIBlueBlock.cpp

#include "PX2UIBlueBlock.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIBlueblock);
PX2_IMPLEMENT_STREAM(UIBlueblock);
PX2_IMPLEMENT_FACTORY(UIBlueblock);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIBlueblock);

//----------------------------------------------------------------------------
UIBlueblock::UIBlueblock()
{
	SetSize(200.0f, 30.0f);
	CreateAddBackgroundPicBox();
}
//----------------------------------------------------------------------------
UIBlueblock::~UIBlueblock()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
UIBlueblock::UIBlueblock(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIBlueblock::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIBlueblock, source);
}
//----------------------------------------------------------------------------
void UIBlueblock::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIBlueblock::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIBlueblock::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UIBlueblock::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIBlueblock, target);
}
//----------------------------------------------------------------------------
int UIBlueblock::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------