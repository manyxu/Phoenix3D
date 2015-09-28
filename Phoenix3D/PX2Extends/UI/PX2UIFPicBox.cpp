// PX2UIFPicBox.cpp

#include "PX2UIFPicBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIFPicBox);
PX2_IMPLEMENT_STREAM(UIFPicBox);
PX2_IMPLEMENT_FACTORY(UIFPicBox);

//----------------------------------------------------------------------------
UIFPicBox::UIFPicBox()
{
	SetName("UIFPicBox");

	mPicBox = new0 UIPicBox();
	AttachChild(mPicBox);
	mPicBox->SetPivot(Float2(0.5f, 0.5f));

	SetSize(Sizef(100.0f, 100.0f));
}
//----------------------------------------------------------------------------
UIFPicBox::~UIFPicBox()
{
}
//----------------------------------------------------------------------------
void UIFPicBox::OnSizeChanged()
{
	if (mBackgroundPicBox)
	{
		mBackgroundPicBox->SetSize(mSize);

		float posX = mSize.Width * mPvoit[0];
		float posZ = mSize.Height * mPvoit[1];
		mBackgroundPicBox->LocalTransform.SetTranslateXZ(posX, posZ);
	}

	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFPicBox::UIFPicBox(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIFPicBox::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mPicBox);

	PX2_END_DEBUG_STREAM_LOAD(UIFPicBox, source);
}
//----------------------------------------------------------------------------
void UIFPicBox::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mPicBox)
	{
		source.ResolveLink(mPicBox);
	}
}
//----------------------------------------------------------------------------
void UIFPicBox::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIFPicBox::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFPicBox::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIFPicBox, target);
}
//----------------------------------------------------------------------------
int UIFPicBox::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//-----------------------------------------------------------------------