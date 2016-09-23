// PX2UISizeExtendControl.cpp

#include "PX2UISizeExtendControl.hpp"
#include "PX2UIFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, UISizeExtendControl);
PX2_IMPLEMENT_STREAM(UISizeExtendControl);
PX2_IMPLEMENT_FACTORY(UISizeExtendControl);

//----------------------------------------------------------------------------
UISizeExtendControl::UISizeExtendControl() :
mWidthOverHeightPercent(1.0f),
mMode(M_SIZEEXTEND_ONLY_ON_BIGGER)
{
}
//----------------------------------------------------------------------------
UISizeExtendControl::~UISizeExtendControl()
{
}
//----------------------------------------------------------------------------
void UISizeExtendControl::SetWidthOverHeightPercent(float percent)
{
	mWidthOverHeightPercent = percent;
}
//----------------------------------------------------------------------------
void UISizeExtendControl::SetControlledable(Controlledable* object)
{
	Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------
void UISizeExtendControl::_Update(double applicationTime, double elapsedTime)
{
	Controller::_Update(applicationTime, elapsedTime);

	UIFrame *parent = DynamicCast<UIFrame>(GetControlledable());
	if (!parent) return;

	_UpdateAlignFrame();
}
//----------------------------------------------------------------------------
void UISizeExtendControl::_UpdateAlignFrame()
{
	UIFrame *frame = DynamicCast<UIFrame>(GetControlledable());

	const Sizef &frameSize = frame->GetSize();
	float canvasWidthOverHeight = frameSize.Width / frameSize.Height;

	float width = frameSize.Width;
	float height = frameSize.Height;

	if (M_SIZEEXTEND == mMode)
	{
		if (canvasWidthOverHeight >= mWidthOverHeightPercent)
		{
			height = frameSize.Height;
			width = height * mWidthOverHeightPercent;
		}
		else
		{
			width = frameSize.Width;
			height = width / mWidthOverHeightPercent;
		}
	}
	else if (M_SIZEEXTEND_ONLY_ON_BIGGER == mMode)
	{
		float originWidth = mOriginHeight * mWidthOverHeightPercent;

		if (originWidth<=width && originWidth<=height)
		{
			width = originWidth;
			height = originWidth;
		}
		else
		{
			if (canvasWidthOverHeight >= mWidthOverHeightPercent)
			{
				height = frameSize.Height;
				width = height * mWidthOverHeightPercent;
			}
			else
			{
				width = frameSize.Width;
				height = width / mWidthOverHeightPercent;
			}
		}
	}

	frame->SetSize(width, height);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISizeExtendControl::UISizeExtendControl(LoadConstructor value) :
Controller(value),
mWidthOverHeightPercent(1.0f),
mOriginHeight(100.0f),
mMode(M_SIZEEXTEND)
{
}
//----------------------------------------------------------------------------
void UISizeExtendControl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mWidthOverHeightPercent);
	source.Read(mOriginHeight);

	PX2_END_DEBUG_STREAM_LOAD(Controller, source);
}
//----------------------------------------------------------------------------
void UISizeExtendControl::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void UISizeExtendControl::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool UISizeExtendControl::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UISizeExtendControl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mWidthOverHeightPercent);
	target.Write(mOriginHeight);

	PX2_END_DEBUG_STREAM_SAVE(Controller, target);
}
//----------------------------------------------------------------------------
int UISizeExtendControl::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mWidthOverHeightPercent);
	size += sizeof(mOriginHeight);

	return size;
}
//----------------------------------------------------------------------------