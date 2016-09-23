// PX2EU_EditFrameTerrain.cpp

#include "PX2EU_EditFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UISizeExtendControl.hpp"
#include "PX2EditorEventType.hpp"
#include "PX2SelectResData.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Edit.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, EU_EditFrame);
PX2_IMPLEMENT_STREAM(EU_EditFrame);
PX2_IMPLEMENT_FACTORY(EU_EditFrame);

//----------------------------------------------------------------------------
EU_EditFrame::EU_EditFrame() :
mEditType(ET_TERRAIN)
{
	ComeInEventWorld();

	mEU_PropertyGridEdit = new0 EU_PropertyGridEdit();
	AttachChild(mEU_PropertyGridEdit);
	mEU_PropertyGridEdit->SetAnchorHor(0.0f, 1.0f);
	mEU_PropertyGridEdit->SetAnchorVer(0.0f, 1.0f);
}
//----------------------------------------------------------------------------
EU_EditFrame::~EU_EditFrame()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void EU_EditFrame::SetEditType(EditType et)
{
	mEditType = et;
}
//----------------------------------------------------------------------------
EU_EditFrame::EditType EU_EditFrame::GetEditType() const
{
	return mEditType;
}
//----------------------------------------------------------------------------
void EU_EditFrame::_RefreshTerrain()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EU_EditFrame::EU_EditFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void EU_EditFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EU_EditFrame, source);
}
//----------------------------------------------------------------------------
void EU_EditFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void EU_EditFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool EU_EditFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EU_EditFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EU_EditFrame, target);
}
//----------------------------------------------------------------------------
int EU_EditFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------