// PX2UIItem.cpp

#include "PX2UIItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIItem);
PX2_IMPLEMENT_STREAM(UIItem);
PX2_IMPLEMENT_FACTORY(UIItem);

//----------------------------------------------------------------------------
UIItem::UIItem() :
mIsNeedReCal(true)
{
}
//----------------------------------------------------------------------------
UIItem::~UIItem()
{
}
//----------------------------------------------------------------------------
void UIItem::SetIconArrowState(IconArrowState state)
{
	mIconArrowState = state;

	mIsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UIItem::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNeedReCal)
	{
		_Recal();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIItem::_Recal()
{


	mIsNeedReCal = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIItem::UIItem(LoadConstructor value) :
UIFrame(value),
mIsNeedReCal(false)
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