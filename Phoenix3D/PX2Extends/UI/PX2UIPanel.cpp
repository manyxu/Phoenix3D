// PX2UIPanel.cpp

#include "PX2UIPanel.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIPanel);
PX2_IMPLEMENT_STREAM(UIPanel);
PX2_IMPLEMENT_FACTORY(UIPanel);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIPanel);

//----------------------------------------------------------------------------
UIPanel::UIPanel(MenuType mt)
{
	mMenuType = mt;

	SetName("UIPanel");
}
//----------------------------------------------------------------------------
UIPanel::~UIPanel()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIPanel::RegistProperties()
{
	UIFrame::RegistProperties();
	AddPropertyClass("UIPanel");
}
//----------------------------------------------------------------------------
void UIPanel::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIPanel::UIPanel(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIPanel::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPanel, source);
}
//----------------------------------------------------------------------------
void UIPanel::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIPanel::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIPanel::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPanel::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIPanel, target);
}
//----------------------------------------------------------------------------
int UIPanel::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------